#!/bin/python
# -*- coding: utf-8 -*-
from Utils import Utils
import pymongo
import time
import socket
from pymongo import MongoClient
from MongoQueue import MongoQueue,MongoJob

class MongoDB(object){
    QUEUE_DB_NAME='queue'
    QUEUE_COL_CRAWLER_NAME='crawler'
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

    def saveReferences(self,refs){
        refs=refs.copy()
        loaded=self.loadReferences()
        for k,_ in refs.items(){
            refs[k]=list(refs[k].union(loaded[k]))
            refs[k].sort()
        }
        if self.dummy{
            ref_path=MongoDB.DUMMY_FOLDER+'references.json'
            Utils.saveJson(ref_path,refs) 
        }else{
            refs={'unique':'unique','refs':refs}
            self.insertOneOnRawDB(refs,'References',index='unique')
        }
    }

    def loadReferences(self){
        if self.dummy{
            ref_path=MongoDB.DUMMY_FOLDER+'references.json'
            found= 1 if Utils.checkIfPathExists(ref_path) else 0
        }else{
            query={'unique':'unique'}
            refs=self.raw_db['References'].find(query)
            found=refs.count()
        }
        if found>0{
            if self.dummy{
                refs=Utils.loadJson(ref_path)
            }else{
                refs=refs.next()['refs']
            }
            for k,_ in refs.items(){
                refs[k]=set(refs[k])
            }
        }else{
            refs={'cve':set(),'cwe':set(),'exploit':set(),'capec':set(),'oval':set()}
        }
        return refs
    }

    def startMongoDatabases(self){
        dblist = self.client.list_database_names()
        if MongoDB.RAW_DATA_DB_NAME not in dblist{
            self.logger.warn('Database {} does not exists, creating it...'.format(MongoDB.RAW_DATA_DB_NAME))
        }
        self.raw_db = self.client[MongoDB.RAW_DATA_DB_NAME]
    }

    def insertOneOnRawDB(self,document,collection,index=None){
        return self.insertManyOnRawDB([document],collection,index=index)
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
                    if result.upserted_id{
                        mod_count+=1
                    }
                }
                self.logger.info('Inserted size: {}...OK'.format(mod_count))
            }else{
                result=collection.update_many(documents)
                self.logger.info('Inserted size: {}...OK'.format(len(result.inserted_ids)))
            }
        }
    }

    def getRawDB(self){
        return self.raw_db
    }

    def startQueue(self,id=0){ 
        consumer_id='crawler_{}-{}'.format(socket.gethostname(),id)
        collection=self.client[MongoDB.QUEUE_DB_NAME][MongoDB.QUEUE_COL_CRAWLER_NAME]
        self.consumer_id=consumer_id
        self.queues={}
        self.queues[MongoDB.QUEUE_COL_CRAWLER_NAME]=MongoQueue(collection, consumer_id=consumer_id, timeout=1500, max_attempts=3)
    }

    def getQueueConsumerId(self){
        return self.consumer_id
    }

    def loopOnQueue(self,crawler){
        while True{
            job=self.queues[MongoDB.QUEUE_COL_CRAWLER_NAME].next()
            if job is not None{
                payload=job.payload
                task=payload['task']
                try{
                    self.logger.info('Running job {}-{}...'.format(task,job.job_id))
                    if task=='DownloadAll'{
                        for db_id in crawler.getAllDatabasesIds(){
                            self.queues[MongoDB.QUEUE_COL_CRAWLER_NAME].put({'task': 'Download','args':{'id':db_id}})
                        }
                    }elif task=='Download'{
                        if payload['args']['id']=='CVE_DETAILS'{
                            if not all(el in self.raw_db.list_collection_names() for el in ['CVE_MITRE','CVE_NVD']){
                                self.logger.warn('Returning {} job to queue, because it does not have its requirements fulfilled'.format(payload['args']['id']))
                                job.put_back()
                                job=None
                                time.sleep(200) # TODO change to 20 and check if there is an active job on queue processing CVE_MITRE or CVE_NVD, if yes wait
                            }
                        }
                        if job{
                            failed=crawler.downloadRawDataFromSources(sources=[payload['args']['id']],update_callback=lambda: job.progress())
                            if failed{
                                raise Exception('Failed to download from {}'.format(','.join(failed)))
                            }
                        }
                    }
                    if job{
                        job.complete()
                        self.logger.info('Runned job {}-{}...'.format(task,job.job_id))
                    }
                }except Exception as e{
                    job.error(str(e))
                    self.logger.error('Runned job {}-{}...'.format(task,job.job_id))      
                    self.logger.exception(e)
                }
            }
        }
    }
}