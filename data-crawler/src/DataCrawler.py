#!/bin/python
# -*- coding: utf-8 -*-

import urllib
import urllib.request
import pandas as pd
import os
import codecs
import re
from Utils import Utils
from XmlDictParser import XmlDictParser

class DataCrawler(object){
    SOURCES=[]
    TMP_FOLDER='/tmp/crawler/'

    def __init__(self, mongo, logger){
        self.logger=logger
		self.mongo=mongo
        self.loadDatabases()
    }
    
    def downloadFromLink(self,link,filename){
        retries=0
        for i in range(retries+1){
            try{
                Utils.createFolderIfNotExists(DataCrawler.TMP_FOLDER)
                if filename.startswith(DataCrawler.TMP_FOLDER){
                    path=filename
                }else{
                    path=DataCrawler.TMP_FOLDER+filename
                }
                self.logger.info('Downloading {} to {}...'.format(link,filename))
                with urllib.request.urlopen(link) as response{
                    if response.code == 200{
                        self.logger.info('HTTP Response Code = 200')
                        piece_size = 4096 # 4 KiB                        
                        with open(path, 'wb') as file{          
                            while True{
                                one_piece=response.read(piece_size)
                                if not one_piece{
                                    break
                                }
                                file.write(one_piece)
                            }
                        }
                        self.logger.info('Downloaded {} to {}...OK'.format(link,filename))
                        return path
                    }else{
                        raise Exception('Response code {} - {}'.format(response.code))
                    }
                }
            } except Exception as e {
                if i>=retries{
                    self.logger.exception(e,fatal=True)
                }else{
                    self.logger.exception(e,fatal=False)
                    self.logger.error('Failed to download ({} of {}) trying again...'.format(i+1,retries+1))
                }
            }
        }
    }

    def loadDatabases(self){
        DataCrawler.SOURCES.append({'id':'CVE_MITRE','index':'cve','direct_download_url':'https://cve.mitre.org/data/downloads/allitems.csv'})
        DataCrawler.SOURCES.append({'id':'CWE_MITRE','index':'cwe','direct_download_url':'https://cwe.mitre.org/data/xml/cwec_latest.xml.zip'})
        # TODO download CVE_MITRE using .gz, to shrink the download size
        # TODO code below parses v
        DataCrawler.SOURCES.append({'id':'CVE_NVD','index':'cve','direct_download_urls':['https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2020.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2019.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2018.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2017.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2016.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2015.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2014.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2013.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2012.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2011.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2010.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2009.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2008.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2007.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2006.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2005.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2004.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2003.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2002.json.zip']})
        DataCrawler.SOURCES.append({'id':'CPE_NVD','index':'cpe','direct_download_url':'https://nvd.nist.gov/feeds/json/cpematch/1.0/nvdcpematch-1.0.json.zip'})
        DataCrawler.SOURCES.append({'id':'OVAL','index':'oval','direct_download_url':'https://oval.cisecurity.org/repository/download/5.11.2/all/oval.xml'})
        DataCrawler.SOURCES.append({'id':'CAPEC_MITRE','index':'capec','direct_download_url':'http://capec.mitre.org/data/xml/capec_latest.xml'})
    }

    def getSourceFromId(self,id){
        for source in DataCrawler.SOURCES{
            if source['id']==id{
                return source
            }
        }
        return None
    }

    def parseDBtoDocuments(self,id,path){
        source=self.getSourceFromId(id)
        documents=[]
        if id=='CVE_MITRE'{
            self.logger.info('Parsing data {} for {}...'.format(path,id))
            columns_size=Utils.countCSVColumns(path)
            df=pd.read_csv(path,header=None,engine='python',names=range(columns_size)).values
            documents.append({source['index']:'__metadata__'})
            documents[0]['CVE Version']=Utils.changeStrDateFormat(re.sub('[^0-9]','', str(df[0][0])),'%Y%m%d','%d/%m/%Y')
            documents[0]['Update Date']=Utils.changeStrDateFormat(re.sub('[^0-9]','', str(df[1][0])),'%Y%m%d','%d/%m/%Y')
            columns=[source['index']]
            for i in range(1,columns_size){
                columns.append(df[2][i])
            }
            df=df[10:] # remove header
            for cve in df{
                cve_entry={}
                for i in range(columns_size){
                    cve_entry[columns[i]]=cve[i]
                }
                documents.append(cve_entry)
            }
            self.logger.info('Parsed data {} for {}...OK'.format(path,id))
        }elif id=='CWE_MITRE'{ # TODO Severe workarounds, is not the ideal solution but xmlschema is not working
            self.logger.info('Parsing {} for {}'.format(path,id))
            xmldict=XmlDictParser.fromFile(path,filter=True)
            self.logger.info('Searching for {} schema...'.format(id))
            possible_schemas=xmldict['schemaLocation'].split(' ')
            for possible_schema in possible_schemas{
                if re.search(r'.*\.xsd$', possible_schema){
                    schema_url=possible_schema
                    break
                }
            }
            if not schema_url{
                raise Exception('Schema not found')
            }
            self.logger.info('Found {} schema...OK'.format(id))
            self.logger.info('Downloading schema for {}...'.format(id))
            schema_path=self.downloadFromLink(schema_url,'{}_schema.xsd'.format(source['id']))
            self.logger.info('Downloaded schema for {}...OK'.format(id))
            xmldict=XmlDictParser.fromFileWithSchema(path,schema_path,filter=True)
            
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'br')
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'style')
            documents.append({source['index']:'__metadata__'})
            documents[0]['CWE Version']=xmldict['Version']
            documents[0]['Update Date']=Utils.changeStrDateFormat(xmldict['Date'],'%Y-%m-%d','%d/%m/%Y')
            for cwe in xmldict['Weaknesses']['Weakness']{
                cwe_entry={}
                for k,v in cwe.items(){
                    if k=='ID'{
                        k=source['index']
                    }
                    cwe_entry[k]=v
                }
                documents.append(cwe_entry)
            }
            for cat in xmldict['Categories']['Category']{
                if 'Relationships' in cat {
                    category_data={'Category':{}}
                    for k,v in cat.items(){
                        if k != 'Relationships'{
                            category_data['Category'][k]=v
                        }
                    }
                    if type(cat['Relationships']['Has_Member']) is dict{
                        cat['Relationships']['Has_Member']=[cat['Relationships']['Has_Member']]
                    }
                    for member in cat['Relationships']['Has_Member']{
                        loc=Utils.getIndexOfDictList(documents,source['index'],member['CWE_ID'])
                        if loc{
                            documents[loc]={**documents[loc],**category_data}
                        }
                    }
                }
            }
            for view in xmldict['Views']['View']{
                if 'Members' in view {
                    view_data={'View':{}}
                    for k,v in view.items(){
                        if k != 'Members'{
                            view_data['View'][k]=v
                        }
                    }
                    if type(view['Members']['Has_Member']) is dict{
                        view['Members']['Has_Member']=[view['Members']['Has_Member']]
                    }
                    for member in view['Members']['Has_Member']{
                        loc=Utils.getIndexOfDictList(documents,source['index'],member['CWE_ID'])
                        if loc{
                            documents[loc]={**documents[loc],**view_data}
                        }
                    }
                }
            }
            for i in range(len(documents)){
                if 'References' in documents[i]{
                    if type(documents[i]['References']['Reference']) is not list{
                        documents[i]['References']['Reference']=[documents[i]['References']['Reference']]
                    }
                    refs=[]
                    for ref in documents[i]['References']['Reference']{
                        ref_if=ref['External_Reference_ID']
                        for ext_ref in xmldict['External_References']['External_Reference']{
                            if ext_ref['Reference_ID']==ref_if{
                                ref_data=ext_ref.copy()
                                ref_data.pop('Reference_ID', None)
                                refs.append(ref_data)
                                break
                            }
                        }
                    }
                    documents[i]['References']=refs
                }
            }
            for i in range(len(documents)){
                if 'Related_Weaknesses' in documents[i]{
                    documents[i]['Related_Weaknesses']=documents[i]['Related_Weaknesses']['Related_Weakness']
                    if type(documents[i]['Related_Weaknesses']) is not list{
                        documents[i]['Related_Weaknesses']=[documents[i]['Related_Weaknesses']]
                    }
                }
                if 'Common_Consequences' in documents[i]{
                    documents[i]['Common_Consequences']=documents[i]['Common_Consequences']['Consequence']
                    if type(documents[i]['Common_Consequences']) is not list{
                        documents[i]['Common_Consequences']=[documents[i]['Common_Consequences']]
                    }
                }
                if 'Demonstrative_Examples' in documents[i]{
                    documents[i]['Demonstrative_Examples']=documents[i]['Demonstrative_Examples']['Demonstrative_Example']
                    if type(documents[i]['Demonstrative_Examples']) is not list{
                        documents[i]['Demonstrative_Examples']=[documents[i]['Demonstrative_Examples']]
                    }
                } 
                if 'Observed_Examples' in documents[i]{
                    documents[i]['Observed_Examples']=documents[i]['Observed_Examples']['Observed_Example']
                    if type(documents[i]['Observed_Examples']) is not list{
                        documents[i]['Observed_Examples']=[documents[i]['Observed_Examples']]
                    }
                } 
                if 'Potential_Mitigations' in documents[i]{
                    documents[i]['Potential_Mitigations']=documents[i]['Potential_Mitigations']['Mitigation']
                    if type(documents[i]['Potential_Mitigations']) is not list{
                        documents[i]['Potential_Mitigations']=[documents[i]['Potential_Mitigations']]
                    }
                }  
                if 'Taxonomy_Mappings' in documents[i]{
                    documents[i]['Taxonomy_Mappings']=documents[i]['Taxonomy_Mappings']['Taxonomy_Mapping']
                    if type(documents[i]['Taxonomy_Mappings']) is not list{
                        documents[i]['Taxonomy_Mappings']=[documents[i]['Taxonomy_Mappings']]
                    }
                } 
                if 'Notes' in documents[i]{
                    documents[i]['Notes']=documents[i]['Notes']['Note']
                    if type(documents[i]['Notes']) is not list{
                        documents[i]['Notes']=[documents[i]['Notes']]
                    }
                } 
                if 'Related_Attack_Patterns' in documents[i]{
                    documents[i]['Related_Attack_Patterns']=documents[i]['Related_Attack_Patterns']['Related_Attack_Pattern']
                    if type(documents[i]['Related_Attack_Patterns']) is not list{
                        documents[i]['Related_Attack_Patterns']=[documents[i]['Related_Attack_Patterns']]
                    }
                }        
                documents[i]=XmlDictParser.compressDictOnFollowingKeys(documents[i],['p','li','ul','div','i'])
                documents[i]=XmlDictParser.recursiveRemoveEmpty(documents[i])
            }
            Utils.deletePath(schema_path)
        }else{
            raise Exception('Unknown id({}).'.format(id))
        }
        return documents
    }

    def downloadRawDataFrom(self,id){
        source=self.getSourceFromId(id)
        if id=='CVE_MITRE'{
            self.logger.info('Downloading CVEs from {}...'.format(id))
            path=self.downloadFromLink(source['direct_download_url'],'{}_all_items.csv'.format(source['id']))
            self.logger.info('Downloaded CVEs from {}...OK'.format(id))
            documents=self.parseDBtoDocuments(id,path)
            return documents, path
        }elif id=='CWE_MITRE'{
            self.logger.info('Downloading CWEs from {}...'.format(id))
            path=self.downloadFromLink(source['direct_download_url'],'{}_cwec_latest.xml.zip'.format(source['id']))
            self.logger.info('Downloaded CWEs from {}...OK'.format(id))
            destination_folder=Utils.unzip(path)
            for file_str in os.listdir(destination_folder){
                if re.search(r'cwe.*\.xml$', file_str){
                    xml_path=destination_folder+file_str
                }
            }
            documents=self.parseDBtoDocuments(id,xml_path)
            return documents, destination_folder            
        }else{
            raise Exception('Unknown id({}).'.format(id))
        }
    }

    def downloadRawDataFromAllSources(self){
        for source in DataCrawler.SOURCES{
            documents,tmp_path=self.downloadRawDataFrom(source['id'])
            self.mongo.insertManyOnRawDB(documents,source['id'],source['index'])
            Utils.deletePath(tmp_path)
        }
    }
}




