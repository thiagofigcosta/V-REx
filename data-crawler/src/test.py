#!/bin/python

from Utils import Utils
from DataCrawler import DataCrawler
from Logger import Logger
from MongoDB import MongoDB
from XmlDictParser import XmlDictParser

Utils.createFolderIfNotExists(DataCrawler.TMP_FOLDER)
LOGGER=Logger(DataCrawler.TMP_FOLDER)
Utils(DataCrawler.TMP_FOLDER,LOGGER)

LOGGER.info('oi')
LOGGER.warn('oi')
LOGGER.error('oi')
try{
    vaidarerro+10
}except Exception as e{
    LOGGER.exception(e)
}
LOGGER.exception(Exception('TEST exception printing'))




mongo=MongoDB.asDummy(LOGGER)

crawler=DataCrawler(mongo,LOGGER)

# documents,tmp_path=crawler.downloadRawDataFrom('CWE_MITRE')
# xmldict=XmlDictParser.fromFileWithSchema('/tmp/crawler/CWE_MITRE_cwec_latest.xml/cwec_v4.2.xml','/tmp/crawler/CWE_MITRE_schema2.xsd')
# mongo.insertManyOnRawDB(documents,'CWE_MITRE','cwe')

# crawler.downloadRawDataFrom('CWE_MITRE')
# out=crawler.parseXMLtoDocuments('CWE_MITRE','/tmp/crawler/CWE_MITRE_cwec_latest.xml/cwec_v4.2.xml')
# mongo.insertManyOnRawDB(out,'CWE_MITRE','cwe')


# documents=crawler.parseCSVtoDocuments('CVE_MITRE','/tmp/crawler/CVE_MITRE_all_items (copy).csv')
# mongo.insertManyOnRawDB(documents,'CVE_MITRE','cve')
crawler.downloadRawDataFromAllSources()

LOGGER.fatal('GG')