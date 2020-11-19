#!/bin/python
# -*- coding: utf-8 -*-
from Utils import Utils
import pymongo
from pymongo import MongoClient
from MongoQueue import MongoQueue,MongoJob

class MongoDB(object){
    QUEUE_DB_NAME='queue'
    QUEUE_COL_CRAWLER_NAME='crawler'
    RAW_DATA_DB_NAME='raw_data'
    PROCESSED_DATA_DB_NAME='processed_data'
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
        if MongoDB.PROCESSED_DATA_DB_NAME not in dblist{
            self.logger.warn('Database {} does not exists, creating it...'.format(MongoDB.PROCESSED_DATA_DB_NAME))
        }
        self.processed_db = self.client[MongoDB.PROCESSED_DATA_DB_NAME]
    }

    def insertOneOnDB(self,db,document,collection,index=None,verbose=True){
        return self.insertManyOnDB(db,[document],collection,index=index,verbose=verbose)
    }

    def insertManyOnDB(self,db,documents,collection,index=None,verbose=True){
        if self.dummy {
            path=MongoDB.DUMMY_FOLDER+'{}.json'.format(collection)
            Utils.saveJson(path,documents)
            return path
        }else{
            if verbose{
                self.logger.info('Inserting on {} db on col: {} with index: {} and size: {}...'.format(db.name,collection,index,len(documents)))
            }
            collection=db[collection]
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
                if verbose{
                    self.logger.info('Inserted size: {}...OK'.format(mod_count)),
                }
            }else{
                result=collection.update_many(documents)
                if verbose{
                    self.logger.info('Inserted size: {}...OK'.format(len(result.inserted_ids)))
                }
            }
        }
    }

    def getRawDB(self){
        return self.raw_db
    }

    def getProcessedDB(self){
        return self.processed_db
    }

    def startQueue(self,id=0){ 
        consumer_id='processor_{}'.format(id)
        collection=self.client[MongoDB.QUEUE_DB_NAME][MongoDB.QUEUE_COL_CRAWLER_NAME]
        self.consumer_id=consumer_id
        self.queues={}
        self.queues[MongoDB.QUEUE_COL_CRAWLER_NAME]=MongoQueue(collection, consumer_id=consumer_id, timeout=1500, max_attempts=3)
    }

    def getQueueConsumerId(self){
        return self.consumer_id
    }

    def getQueueNames(self){
        return self.client[MongoDB.QUEUE_DB_NAME].collection_names()
    }

    def clearQueue(self,name){
        return self.queues[name].clear()
    }

    def getAllQueueJobs(self){
        out={}
        queues=self.getQueueNames()
        for queue_name in queues{
            queue=self.client[MongoDB.QUEUE_DB_NAME][queue_name]
            jobs=queue.find({})
            parsed_jobs=[]
            for job in jobs{
                job_dict={'task':job['payload']['task']}
                if 'args' in job['payload']{
                    if job['payload']['args']{
                        job_dict['args']=job['payload']['args']
                    }
                }
                if job['locked_by'] is None{
                    status='WAITING'
                }else{
                    status='RUNNING'
                    locked_at=job['locked_at']
                    end_date=locked_at+datetime.timedelta(0,self.queues[queue_name].timeout+3)
                    if end_date<datetime.datetime.now(){
                        query={"_id": job['_id']}
                        if job['attempts'] > self.queues[queue_name].max_attempts{
                            status='FAILED ALL ATTEMPS'
                            queue.remove(query)
                        }else{
                            status='FAILED'
                        }
                        time_failed=datetime.datetime.now()-end_date
                        if datetime.timedelta(hours=2)<time_failed{
                            status='CANCELING'
                            update={"$set": {"locked_by": None, "locked_at": None},"$inc": {"attempts": 1}}
                            queue.find_and_modify(query,update=update)
                        }
                    }
                }
                job_dict['status']=status
                if status=='RUNNING'{
                    job_dict['worker']=job['locked_by']
                }
                job_dict['attemps']=job['attempts']
                if job['last_error']{
                    job_dict['error']=job['last_error']
                }
                job_dict['id']=job['_id']
                parsed_jobs.append(job_dict)
            }
            if len(parsed_jobs)>0{
                out[queue_name]=parsed_jobs
            }
        }
        return out
    }

    def insertOnCrawlerQueue(self,task,args=None){
        payload={'task': task}
        if args{
            payload['args']=args
        }
        self.queues[MongoDB.QUEUE_COL_CRAWLER_NAME].put(payload)
    }

    def findOneOnDB(self,db,collection,query){
        if self.dummy{
            raise Exception('Find one is not supported on DummyMode')
        }
        result=db[collection].find(query)
        found=result.count()
        if found>0{
            return result.next()
        }
    }

    def findOneOnDBFromIndex(self,db,collection,index_field,index_value){
        query={index_field:index_value}
        return self.findOneOnDB(db,collection,query)
    }
}