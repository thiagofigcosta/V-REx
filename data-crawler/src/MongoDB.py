#!/bin/python
# -*- coding: utf-8 -*-
from Utils import Utils
from pymongo import MongoClient
# from mongoqueue import MongoQueue

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
        if RAW_DATA_DB_NAME not in dblist{
            self.logger.warn('Database {} does not exists, creating it...'.format(RAW_DATA_DB_NAME))
        }
        self.raw_db = self.client[RAW_DATA_DB_NAME]
    }

    def insertManyOnRawDB(self,documents,collection,index=None){
        if self.dummy {
            path=MongoDB.DUMMY_FOLDER+'{}.json'.format(collection)
            Utils.saveJson(path,documents)
            return path
        }else{
            self.logger.info('Inserting on raw db col:{} index:{} size:{}...'.format(collection,index,len(documents)))
            collection=self.raw_db[collection]
            if index is not None{
                collection.createIndex([(index, pymongo.ASCENDING)], unique=True)
            }
            result=collection.insert_many(documents)
            self.logger.info('Inserted size: {}...OK'.format(len(result))) # TODO choose
            self.logger.info('Inserted:\n{}'.format(result)) # TODO choose
        }
    }

    def getRawDB(self){
        return self.raw_db
    }

    def startQueue(self,id=0){ 
        # TODO fix queue, add manually sources, pip code not working
        #https://github.com/kapilt/mongoqueue/blob/master/mongoqueue/test.py
        #https://github.com/kapilt/mongoqueue/blob/master/mongoqueue/mongoqueue.py
        #https://github.com/kapilt/mongoqueue/blob/master/mongoqueue/lock.py
        #https://github.com/kapilt/mongoqueue/blob/master/mongoqueue/__init__.py
        
        
        # https://pypi.org/project/mongoqueue/ http://learnmongodbthehardway.com/schema/queues/
        consumer_id='consumer_{}'.format(id)
        #collection=self.client.task_queue # TODO test
        collection=self.client[QUEUE_DB_NAME][QUEUE_COL_NAME]
        # self.queue = MongoQueue(collection, consumer_id=consumer_id, timeout=600, max_attempts=3)
    }

    def loop(self){
        while True{
            job=self.queue.next()
            if job is not None{
                try{
                    print(job)
                    # TODO process job
                    # if job is to download every source use getAllDatabasesId to split it into several jobs, one for each id and them publish all of them of queue
                    job.complete()
                }except Exception as e{
                    # TODO log exception                    
                    job.error(str(e))
                }
            }
        }
    }
}