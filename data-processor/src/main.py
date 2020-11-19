#!/bin/python

from DataProcessor import DataProcessor
from Utils import Utils
from Logger import Logger
from MongoDB import MongoDB

Utils.createFolderIfNotExists(DataProcessor.TMP_FOLDER)
LOGGER=Logger(DataProcessor.TMP_FOLDER)
Utils(DataProcessor.TMP_FOLDER,LOGGER)

LOGGER.info('Starting Data Processor...')
mongo=MongoDB('127.0.0.1',27017,LOGGER,user='root',password='123456')
mongo.startQueue(id=0) # TODO get ID from enviroment var
LOGGER.info('Started Data Processor...OK')
LOGGER.info('Writting on queue as {}'.format(mongo.getQueueConsumerId()))