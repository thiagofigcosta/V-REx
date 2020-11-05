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
# out=crawler.parseDBtoDocuments('CWE_MITRE','/tmp/crawler/CWE_MITRE_cwec_latest.xml/cwec_v4.2.xml')
# mongo.insertManyOnRawDB(out,'CWE_MITRE','cwe')

# crawler.downloadRawDataFrom('CVE_NVD')
# out=crawler.parseDBtoDocuments('CVE_NVD',['/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2005.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2020.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2012.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2007.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2010.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2009.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2011.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2003.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2016.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2004.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2015.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2006.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2017.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2019.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2008.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2002.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2014.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2013.json', '/tmp/crawler/CVE_NVD_unziped/nvdcve-1.1-2018.json'])
# mongo.insertManyOnRawDB(out,'CVE_NVD','cve')

# crawler.downloadRawDataFrom('CVE_MITRE')
# documents=crawler.parseCSVtoDocuments('CVE_MITRE','/tmp/crawler/CVE_MITRE_all_items (copy).csv')
# mongo.insertManyOnRawDB(documents,'CVE_MITRE','cve')


# crawler.downloadRawDataFromAllSources()

LOGGER.fatal('GG')