#!/bin/python
# -*- coding: utf-8 -*-
from Utils import Utils
import pymongo
from pymongo import MongoClient
from MongoQueue import MongoQueue,MongoJob

class MongoDB(object){
    QUEUE_DB_NAME='queue'
    QUEUE_COL_NAME='crawler'
    RAW_DATA_DB_NAME='raw_data'
    DUMMY_FOLDER='tmp/crawler/DummyMongo/'

	def __init__(self, address, port=27017, logger=None, user=None,password=None){
        if logger is None{
            raise Exception('MongoDB logger cannot be None')
        }
        self.logger=logger
        if address is None and port is None{
            self.dummy=True
            Utils.createFolderIfNotExists(MongoDB.DUMMY_FOLDER)
        }else{
            self.dummy=False
            self.address=address
            self.port=port
            self.user=user 
            self.password=password
            self.logger.info('Preparing mongo on {}:{}...'.format(address,port))
            self.establishConnection()
            self.startMongoDatabases()
            self.logger.info('Prepared mongo on {}:{}...OK'.format(address,port))
        }
    }

    @classmethod
    def asDummy(cls,logger){
        return cls(address=None,port=None,logger=logger)
    }

    def establishConnection(self){
        client = MongoClient(self.address, self.port,username=self.user, password=self.password)
        self.client=client
    }

    def startMongoDatabases(self){
        dblist = self.client.list_database_names()
        if MongoDB.RAW_DATA_DB_NAME not in dblist{
            self.logger.warn('Database {} does not exists, creating it...'.format(MongoDB.RAW_DATA_DB_NAME))
        }
        self.raw_db = self.client[MongoDB.RAW_DATA_DB_NAME]
    }

    def insertManyOnRawDB(self,documents,collection,index=None){
        if self.dummy {
            path=MongoDB.DUMMY_FOLDER+'{}.json'.format(collection)
            Utils.saveJson(path,documents)
            return path
        }else{
            self.logger.info('Inserting on raw db col:{} index: {} size: {}...'.format(collection,index,len(documents)))
            collection=self.raw_db[collection]
            if index is not None{
                collection.create_index([(index, pymongo.ASCENDING)], unique=True)
                mod_count=0
                for doc in documents{
                    query={index: doc[index]}
                    result=collection.replace_one(query, doc, upsert=True) 
                    if result.modified_count > 0{
                        mod_count+=result.modified_count
                    }
                }
                self.logger.info('Inserted size: {}...OK'.format(mod_count))
            }else{
                result=collection.update_many(documents,upsert=True)
                self.logger.info('Inserted size: {}...OK'.format(len(result.inserted_ids)))
            }
        }
    }

    def getRawDB(self){
        return self.raw_db
    }

    def startQueue(self,id=0){ 
        consumer_id='consumer_{}'.format(id)
        collection=self.client[MongoDB.QUEUE_DB_NAME][MongoDB.QUEUE_COL_NAME]
        self.consumer_id=consumer_id
        self.queue=MongoQueue(collection, consumer_id=consumer_id, timeout=1500, max_attempts=3)
    }

    def getQueueConsumerId(self){
        return self.consumer_id
    }

    def loopOnQueue(self,crawler){
        while True{
            job=self.queue.next()
            if job is not None{
                payload=job.payload
                task=payload['task']
                try{
                    self.logger.info('Running job {}...'.format(task))
                    if task=='DownloadAll'{
                        for db_id in crawler.getAllDatabasesIds(){
                            self.queue.put({'task': 'Download','args':{'id':db_id}})
                        }
                    }elif task=='Download'{
                        crawler.downloadRawDataFromSources(sources=[payload['args']['id']],update_callback=lambda: job.progress())
                    }
                    job.complete()
                    self.logger.info('Runned job {}...OK'.format(task))
                }except Exception as e{
                    job.error(str(e))
                    self.logger.error('Runned job {}...FAILED'.format(task))          
                    self.logger.exception(e)
                }
            }
        }
    }
}