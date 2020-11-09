#!/bin/python
# -*- coding: utf-8 -*-

import urllib
import urllib.request
import pandas as pd
import os
import codecs
import re
import time
from Utils import Utils
from XmlDictParser import XmlDictParser

class DataCrawler(object){
    SOURCES=[]
    TMP_FOLDER='tmp/crawler/'

    def __init__(self, mongo, logger){
        self.logger=logger
		self.mongo=mongo
        self.loadDatabases()
        self.loadReferences()
    }

    def loadReferences(self){
        # TODO use mongo
        ref_path=DataCrawler.TMP_FOLDER+'references.json'
        if Utils.checkIfPathExists(ref_path){
            self.references=Utils.loadJson(ref_path)
            for k,_ in self.references.items(){
                self.references[k]=set(self.references[k])
            }
        }else{
            self.references={'cve':set(),'cwe':set(),'exploit':set()}
        }
    }

    def saveReferences(self){
        references_copy=self.references.copy()
        for k,_ in references_copy.items(){
            references_copy[k]=list(references_copy[k])
        }
        # TODO use mongo
        Utils.saveJson(DataCrawler.TMP_FOLDER+'references.json',references_copy) 
    }
    
    def downloadFromLink(self,link,filename,timeout=600){
        retries=3
        for i in range(retries+1){
            try{
                Utils.createFolderIfNotExists(DataCrawler.TMP_FOLDER)
                if filename.startswith(DataCrawler.TMP_FOLDER){
                    path=filename
                }else{
                    path=DataCrawler.TMP_FOLDER+filename
                }
                self.logger.info('Downloading {} to {}...'.format(link,filename))
                with urllib.request.urlopen(link,timeout=timeout) as response{
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
                        raise Exception('Response code {} - {}'.format(response.code),response.code)
                    }
                }
            } except Exception as e {
                if str(e)=='HTTP Error 404: Not Found' or i>=retries{
                    raise e
                }else{
                    self.logger.exception(e,fatal=False)
                    self.logger.error('Failed to download ({} of {}) trying again...'.format(i+1,retries))
                }
            }
        }
    }

    def loadDatabases(self){
        DataCrawler.SOURCES.append({'id':'CVE_MITRE','index':'cve','direct_download_url':'https://cve.mitre.org/data/downloads/allitems.csv.gz'})
        DataCrawler.SOURCES.append({'id':'CWE_MITRE','index':'cwe','direct_download_url':'https://cwe.mitre.org/data/xml/cwec_latest.xml.zip'})
        DataCrawler.SOURCES.append({'id':'CVE_NVD','index':'cve','direct_download_urls':['https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2020.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2019.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2018.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2017.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2016.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2015.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2014.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2013.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2012.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2011.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2010.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2009.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2008.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2007.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2006.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2005.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2004.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2003.json.zip','https://nvd.nist.gov/feeds/json/cve/1.1/nvdcve-1.1-2002.json.zip']})
        DataCrawler.SOURCES.append({'id':'CAPEC_MITRE','index':'capec','direct_download_url':'http://capec.mitre.org/data/archive/capec_latest.zip'})
        DataCrawler.SOURCES.append({'id':'OVAL','index':'oval','direct_download_url':'https://oval.cisecurity.org/repository/download/5.11.2/all/oval.xml.zip'})
        DataCrawler.SOURCES.append({'id':'EXPLOIT_DB','index':'exploit','base_download_url':'https://www.exploit-db.com/exploits/'})
        # TODO other sources
        # CVE DETAILS ALREADY HAS DATA AGGREGATED FROM MULTIPLES SOURCES
        # https://www.cvedetails.com/
        # https://www.kb.cert.org/vuls/search/
        # https://www.securityfocus.com/vulnerabilities
        # https://www.broadcom.com/support/security-center/attacksignatures
        # ????????? Rapid 7’s Metasploit, D2 Security’s Elliot Kit and Canvas Exploitation Framework, OpenVAS
    }

    def getSourceFromId(self,id){
        for source in DataCrawler.SOURCES{
            if source['id']==id{
                return source
            }
        }
        return None
    }

    def parseDBtoDocuments(self,id,paths){
        source=self.getSourceFromId(id)
        if type(paths) is str{
            path=paths
        }elif type(paths) is list{
            path='Multiple files at {}'.format(Utils.partentFromPath(paths[0]))
        }else{
            raise Exception('Unknown data type for paths on parseDBtoDocuments. Type was {}'.format(type(paths)))
        }
        self.logger.info('Parsing data {} for {}...'.format(path,id))
        documents=[]
        if id=='CVE_MITRE'{
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
            documents[0]['Data Count']=len(df)
            for cve in df{
                cve_entry={}
                for i in range(columns_size){
                    cve_entry[columns[i]]=cve[i]
                    if columns[i]==source['index']{
                        ref=cve[i].replace('CVE-', '')
                        self.references['cve'].add(ref)
                    }
                }
                documents.append(cve_entry)
            }
        }elif id=='CWE_MITRE'{ # TODO Severe workarounds, it is not the ideal solution but xmlschema is not working
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
            xmldict=XmlDictParser.fromFileWithSchema2(path,schema_path,filter=True)
            
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'br')
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'style')
            documents.append({source['index']:'__metadata__'})
            documents[0]['CWE Version']=xmldict['Version']
            documents[0]['Update Date']=Utils.changeStrDateFormat(xmldict['Date'],'%Y-%m-%d','%d/%m/%Y')
            documents[0]['Data Count']=len(xmldict['Weaknesses']['Weakness'])
            for cwe in xmldict['Weaknesses']['Weakness']{
                cwe_entry={}
                for k,v in cwe.items(){
                    if k=='ID'{
                        k=source['index']
                        self.references['cwe'].add(v)
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
            #prettify
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
        }elif id=='CVE_NVD'{
            documents.append({source['index']:'__metadata__'})
            documents[0]['CVE Version']=[]
            documents[0]['Update Date']=[]
            documents[0]['Data Count']=0
            for json_path in paths{
                cves_data=Utils.loadJson(json_path)
                if cves_data['CVE_data_version'] not in documents[0]['CVE Version']{
                    documents[0]['CVE Version'].append(cves_data['CVE_data_version'])
                }
                if cves_data['CVE_data_timestamp'] not in documents[0]['Update Date']{
                    documents[0]['Update Date'].append(cves_data['CVE_data_timestamp'])
                }
                documents[0]['Data Count']+=int(cves_data['CVE_data_numberOfCVEs'])
                for cve_data in cves_data['CVE_Items']{
                    cve_entry={}
                    for k,v in cve_data.items(){
                        if k=='cve'{
                           cve_entry[source['index']]=v['CVE_data_meta']['ID']
                           ref=v['CVE_data_meta']['ID'].replace('CVE-', '')
                           self.references['cve'].add(ref)
                           for k2,v2 in v.items(){
                               if k2 not in ('data_type','data_format','data_version','CVE_data_meta'){
                                    if k2=='references'{
                                        v2=v2['reference_data']
                                    }
                                    cve_entry[k2]=v2
                               }
                           }
                        }else{
                            if k=='configurations'{
                                v=v['nodes']
                            }
                            cve_entry[k]=v
                        }
                    }
                    if source['index'] in cve_entry{
                        documents.append(cve_entry)
                    }else{
                        raise Exception('No CVE id found on source {} path {}'.format(id,json_path))
                    }
                }
            }
        }elif id=='CAPEC_MITRE'{
            xmldict=XmlDictParser.fromFileWithSchema2(paths[0],paths[1],filter=True)
            xmldict=XmlDictParser.stringfyDict(xmldict)
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'br')
            xmldict=XmlDictParser.recursiveRemoveKey(xmldict,'style')
            documents.append({source['index']:'__metadata__'})
            documents[0]['CAPEC Version']=xmldict['Version']
            documents[0]['Update Date']=Utils.changeStrDateFormat(xmldict['Date'],'%Y-%m-%d','%d/%m/%Y')
            documents[0]['Data Count']=len(xmldict['Attack_Patterns']['Attack_Pattern'])
            for capec in xmldict['Attack_Patterns']['Attack_Pattern']{
                capec_entry={}
                for k,v in capec.items(){
                    if k=='ID'{
                        k=source['index']
                    }
                    capec_entry[k]=v
                }
                documents.append(capec_entry)
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
                        loc=Utils.getIndexOfDictList(documents,source['index'],member['CAPEC_ID'])
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
                        loc=Utils.getIndexOfDictList(documents,source['index'],member['CAPEC_ID'])
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
            #prettify
            for i in range(len(documents)){
                if 'Related_Weaknesses' in documents[i]{
                    documents[i]['Related_Weaknesses']=documents[i]['Related_Weaknesses']['Related_Weakness']
                    if type(documents[i]['Related_Weaknesses']) is not list{
                        documents[i]['Related_Weaknesses']=[documents[i]['Related_Weaknesses']]
                    }
                }
                if 'Related_Attack_Patterns' in documents[i]{
                    documents[i]['Related_Attack_Patterns']=documents[i]['Related_Attack_Patterns']['Related_Attack_Pattern']
                    if type(documents[i]['Related_Attack_Patterns']) is not list{
                        documents[i]['Related_Attack_Patterns']=[documents[i]['Related_Attack_Patterns']]
                    }
                }
                if 'Execution_Flow' in documents[i]{
                    documents[i]['Execution_Flow']=documents[i]['Execution_Flow']['Attack_Step']
                    if type(documents[i]['Execution_Flow']) is not list{
                        documents[i]['Execution_Flow']=[documents[i]['Execution_Flow']]
                    }
                } 
                if 'Notes' in documents[i]{
                    documents[i]['Notes']=documents[i]['Notes']['Note']
                    if type(documents[i]['Notes']) is not list{
                        documents[i]['Notes']=[documents[i]['Notes']]
                    }
                }  
                if 'Consequences' in documents[i]{
                    documents[i]['Consequences']=documents[i]['Consequences']['Consequence']
                    if type(documents[i]['Consequences']) is not list{
                        documents[i]['Consequences']=[documents[i]['Consequences']]
                    }
                } 
                if 'Mitigations' in documents[i]{
                    documents[i]['Mitigations']=documents[i]['Mitigations']['Mitigation']
                    if type(documents[i]['Mitigations']) is not list{
                        documents[i]['Mitigations']=[documents[i]['Mitigations']]
                    }
                }   
                if 'Prerequisites' in documents[i]{
                    documents[i]['Prerequisites']=documents[i]['Prerequisites']['Prerequisite']
                    if type(documents[i]['Prerequisites']) is not list{
                        documents[i]['Prerequisites']=[documents[i]['Prerequisites']]
                    }
                }  
                if 'Skills_Required' in documents[i]{
                    documents[i]['Skills_Required']=documents[i]['Skills_Required']['Skill']
                    if type(documents[i]['Skills_Required']) is not list{
                        documents[i]['Skills_Required']=[documents[i]['Skills_Required']]
                    }
                }       
                documents[i]=XmlDictParser.compressDictOnFollowingKeys(documents[i],['p','li','ul','div','i','class'])
                documents[i]=XmlDictParser.recursiveRemoveEmpty(documents[i])
            }
        }elif id=='OVAL'{
            xmldict=XmlDictParser.fromFile(path,filter=True)
            documents.append({source['index']:'__metadata__'})
            documents[0]['OVAL Version']=xmldict['generator']['schema_version']
            documents[0]['Update Date']=Utils.changeStrDateFormat(xmldict['generator']['timestamp'],'%Y-%m-%dT%H:%M:%S','%d/%m/%Y')
            documents[0]['Data Count']=len(xmldict['definitions']['definition'])
            
            for definition in xmldict['definitions']['definition']{
                oval_entry={}
                add_entry=True
                for k,v in definition.items(){
                    if k=='id'{
                        k=source['index']
                    }
                    if k=='metadata'{
                        for k2,v2 in v.items(){
                            oval_entry[k2]=v2
                            if k2=='oval_repository'{
                                for k3,v3 in v2['dates'].items(){
                                    if k3=='status_change'{
                                        if type(v3) is list{
                                            last_el=v3[len(v3)-1]
                                        }else{
                                            last_el=v3
                                        }
                                        if  'status_change' in last_el and last_el['status_change']=='DEPRECATED'{
                                            add_entry=False
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        oval_entry[k]=v
                    }
                }
                if add_entry{
                    documents.append(oval_entry)
                }
            }
        }elif id=='EXPLOIT_DB'{
            documents.append({source['index']:'__metadata__'})
            documents[0]['Update Date']=Utils.getTodayDate()
            documents[0]['Data Count']=len(paths)
            for path in paths {
                exploit_entry={}
                raw_html=Utils.openFile(path)
                patterns={source['index']:r'[.|\n]*EDB-ID:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','cve':r'[.|\n]*CVE:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','author':r'[.|\n]*Author:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','type':r'[.|\n]*Type:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','platform':r'[.|\n]*Platform:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','date':r'[.|\n]*Date:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','vulnerable':r'[.|\n]*Vulnerable App:[\s|\n]*<\/.*>[\s|\n]*<.*>((\n|.)*?)<\/.*>[.|\n]*','verified':r'[.|\n]*EDB Verified:[\s|\n]*<\/.*>[\s|\n]*(<.*\n.*>)[\s|\n]*','code':r'.*<code.*>((.|\n)*)<\/code>.*'}
                for k,pattern in patterns.items(){
                    result=re.search(pattern, raw_html, re.MULTILINE)
                    v=result.group(1)
                    if 'mdi-close' in v{
                        v='False'
                    }elif 'mdi-check' in v{
                        v='True'
                    }
                    v=re.sub(r'<.*>','',v).strip()
                    if v and v!='N/A'{
                        exploit_entry[k]=v
                    }
                }                
                documents.append(exploit_entry)
            }
        }else{
            raise Exception('Unknown id({}).'.format(id))
        }
        self.logger.info('Parsed data {} for {}...OK'.format(path,id))
        return documents
    }

    def downloadRawDataAndParseFrom(self,id){
        source=self.getSourceFromId(id)
        if id=='CVE_MITRE'{
            self.logger.info('Downloading CVEs from {}...'.format(id))
            path=self.downloadFromLink(source['direct_download_url'],'{}_all_items.csv.gz'.format(source['id']))
            self.logger.info('Downloaded CVEs from {}...OK'.format(id))
            destination_folder=Utils.gunzip(path,'')
            for file_str in os.listdir(destination_folder){
                if re.search(r'.*\.csv$', file_str){
                    csv_path=destination_folder+file_str
                }
            }
            documents=self.parseDBtoDocuments(id,csv_path)
            return documents, destination_folder
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
        }elif id=='CVE_NVD'{
            self.logger.info('Downloading CWEs from {}...'.format(id))
            urls = source['direct_download_urls']
            paths = []
            for url in urls{
                paths.append(self.downloadFromLink(url,Utils.filenameFromPath(url,get_extension=True)))
            }
            self.logger.info('Downloaded CWEs from {}...OK'.format(id))
            for path in paths{
                 destination_folder=Utils.unzip(path,destination_folder='CVE_NVD_unziped/')
            }
            json_files=[]
            for file_str in os.listdir(destination_folder){
                if re.search(r'.*\.json$', file_str){
                    json_files.append(destination_folder+file_str)
                }
            }
            documents=self.parseDBtoDocuments(id,json_files)
            return documents, destination_folder
        }elif id=='CAPEC_MITRE'{
            self.logger.info('Downloading CAPECs from {}...'.format(id))
            path=self.downloadFromLink(source['direct_download_url'],'{}_latest.zip'.format(source['id']))
            self.logger.info('Downloaded CAPECs from {}...OK'.format(id))
            destination_folder=Utils.unzip(path)
            for file_str in os.listdir(destination_folder){
                if re.search(r'.*\.xml$', file_str){
                    xml_path=destination_folder+file_str
                }
                if re.search(r'.*\.xsd$', file_str){
                    xsd_path=destination_folder+file_str
                }
            }
            documents=self.parseDBtoDocuments(id,[xml_path,xsd_path])
            return documents, destination_folder            
        }elif id=='OVAL'{
            self.logger.info('Downloading OVALs from {}...'.format(id))
            path=self.downloadFromLink(source['direct_download_url'],'{}_all.zip'.format(source['id']))
            self.logger.info('Downloaded OVALs from {}...OK'.format(id))
            destination_folder=Utils.unzip(path)
            for file_str in os.listdir(destination_folder){
                if re.search(r'.*\.xml$', file_str){
                    xml_path=destination_folder+file_str
                }
            }
            documents=self.parseDBtoDocuments(id,xml_path)
            return documents, destination_folder            
        }elif id=='EXPLOIT_DB'{
            self.logger.info('Downloading EXPLOITs from {}...'.format(id))
            destination_folder=Utils.getTmpFolder(id)
            exploit_id=1
            max_known_exploit=48999
            downloaded=[]
            for file_str in os.listdir(destination_folder){
                result=re.search(r'.*id-([0-9]+)\..*$', file_str)
                if result{
                    downloaded.append(int(result.group(1)))
                }
                if re.search(r'.*id-([0-9]+)\.DELETEME$', file_str){
                    Utils.deletePath(destination_folder+file_str)
                }
            }
            if len(downloaded)>0{
                exploit_id=max(downloaded)+1
            }
            first_timeout=True
            while True{
                try{
                    path=self.downloadFromLink(source['base_download_url']+str(exploit_id),destination_folder+'{}_id-{}.html'.format(source['id'],exploit_id),timeout=120)
                    first_timeout=True
                    self.references['exploit'].add(exploit_id)
                }except Exception as e {
                    if exploit_id>max_known_exploit{
                        if str(e)=='HTTP Error 404: Not Found'{
                            self.logger.info('Last exploit found, id {}'.format(exploit_id-1))
                            break
                        }else{
                            self.logger.exception(e,fatal=False)
                        }
                    }else{
                        if str(e)=='HTTP Error 404: Not Found'{
                            self.logger.warn('Exploit with id {} not found. 404. Keep going until: {}'.format(exploit_id,max_known_exploit))
                        }else{
                            if first_timeout{
                                first_timeout=False
                                self.logger.warn('Server probably blocked access temporally on id {}. Waiting 20 Minutes...'.format(exploit_id))
                                exploit_id-=1
                                time.sleep(1200)
                            }else{  
                                self.logger.warn('Downloaded EXPLOITs from {}...FAILED. Server probably blocked access temporally. Aborting...'.format(exploit_id))
                                self.logger.exception(e,fatal=False)
                                documents=None
                                Utils.saveFile(destination_folder+'{}_id-{}.DELETEME'.format(source['id'],exploit_id-1),'') # checkpoint
                                return documents, destination_folder
                            }
                        }
                    }
                }
                exploit_id+=1
            }
            self.logger.info('Downloaded EXPLOITs from {}...OK'.format(id))
            paths=[]
            for file_str in os.listdir(destination_folder){
                if re.search(r'.*\.html$', file_str){
                    paths.append(destination_folder+file_str)
                }
            }
            documents=self.parseDBtoDocuments(id,paths)
            return documents, destination_folder 
        }else{ 
            raise Exception('Unknown id({}).'.format(id))
        }
    }

    def downloadRawDataFromSources(self,sources=None){
        failed=[]
        for source in DataCrawler.SOURCES{
            if sources is None or source['id'] in sources{
                try{
                    documents,tmp_path=self.downloadRawDataAndParseFrom(source['id'])
                    if documents is not None{
                        self.mongo.insertManyOnRawDB(documents,source['id'],source['index'])
                        Utils.deletePath(tmp_path)
                    }else{
                        failed.append(source['id'])
                    }
                } except Exception as e {
                    self.logger.exception(e,fatal=False)
                    failed.append(source['id'])
                }finally{
                    self.saveReferences()
                }
            }
        }
        if len(failed)>0{
            return failed
        }
    }
}




