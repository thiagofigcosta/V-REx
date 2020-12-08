#!/bin/python
# -*- coding: utf-8 -*-

from Utils import Utils
import re

class DataProcessor(object){
    TMP_FOLDER='tmp/processor/'

    def __init__(self, mongo, logger){
        self.logger=logger
		self.mongo=mongo
        self.references=self.mongo.loadReferences()
    }

    def mergeCve(self,update_callback=None){
        raw_db=self.mongo.getRawDB()
        keys_source={'CVE_MITRE':set(),'CVE_NVD':set(),'CVE_DETAILS':set()}
        cve_collections=[k for k,_ in keys_source.items()]
        if not all(el in raw_db.list_collection_names() for el in cve_collections){
            raise Exception('Mongo does not contains every needed collection: {}'.format(' - '.join(cve_collections)))
        }
        verbose_frequency=666
        iter_count=1
        frequency_count=1
        data_size=0
        total_iters=len(self.references['cve'])
        self.logger.info('Running \"Merge\" on CVEs Data...')
        for cve_ref in self.references['cve']{
            merged_entry={}
            for col in cve_collections{
                data=self.mongo.findOneOnDBFromIndex(raw_db,col,'cve','CVE-{}'.format(cve_ref))
                if data{
                    for k,v in data.items(){
                        if k.lower()=='references'{
                            k='references-{}'.format(col)
                        }
                        if k.lower()=='description'{
                            k='description-{}'.format(col)
                        }
                        if k!= '_id'{
                            merged_entry[k]=v
                            keys_source[col].add(k)
                        }
                    }
                }
            }
            if len(merged_entry)>0{
                self.mongo.insertOneOnDB(self.mongo.getProcessedDB(),merged_entry,'merged_cve','cve',verbose=False)
                data_size+=Utils.sizeof(merged_entry)
                if update_callback { update_callback() }
                if verbose_frequency==frequency_count{
                    self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((iter_count/total_iters*100),Utils.bytesToHumanReadable(data_size)))
                    frequency_count=0
                }
            }
            iter_count+=1
            frequency_count+=1
        }
        self.logger.info('Percentage done {:.2f}% - Total data size: {}'.format((iter_count/total_iters*100),Utils.bytesToHumanReadable(data_size)))
        self.logger.info('Runned \"Merge\" on CVEs Data...OK')
    }

    def flatternAndSimplifyCve(self,update_callback=None){
        # cve - OK
        # Status - OK
        # description-CVE_MITRE - OK
        # references-CVE_MITRE - OK
        # Phase - OK
        # Votes - OK
        # problemtype
        # references-CVE_NVD - OK
        # description-CVE_NVD - OK
        # configurations
        # impact
        # publishedDate
        # lastModifiedDate
        # cvss score
        # confidentiality imp.
        # integrity imp.
        # availability imp.
        # complexity
        # authentication
        # gained acc.
        # vul. type
        # publish date
        # last mod date
        # description-CVE_DETAILS - OK
        # prod. affected
        # versions affected
        # references-CVE_DETAILS - OK
        # cwe
        # metasploitable
        # Comments
        self.logger.info('Running \"Flattern and Simplify\" on CVEs Data...')
        merged_data=self.mongo.findAllOnDB(self.mongo.getProcessedDB(),'merged_cve')
        tmp=set() # TODO remove me
        maxs=0
        for cve in merged_data{
            # References
            cve['References']=[]
            cve['References-class']=[]
            if 'references-CVE_NVD' in cve{
                refs_mitre=cve['references-CVE_NVD']
                for ref in refs_mitre{
                    ref_url=ref['url'].strip()
                    if ref_url not in cve['References']{
                        cve['References'].append(ref_url)
                        cve['References-class'].append(ref['tags'])
                    }else{
                        ref_idx=cve['References'].index(ref_url)
                        for tag in ref['tags']{
                            if tag.strip() not in cve['References-class'][ref_idx]{
                                cve['References-class'][ref_idx].append(tag.strip())
                            }
                        }
                    }
                }
                cve.pop('references-CVE_NVD', None)
            }
            if 'references-CVE_MITRE' in cve{
                refs_mitre=cve['references-CVE_MITRE'].split('|')
                for ref in refs_mitre{
                    ref=ref.split(':',1)
                    ref_url=ref[1].strip()
                    if ref_url not in cve['References']{
                        cve['References'].append(ref_url)
                        cve['References-class'].append([ref[0].strip()])
                    }else{
                        ref_idx=cve['References'].index(ref_url)
                        if ref[0].strip() not in cve['References-class'][ref_idx]{
                            cve['References-class'][ref_idx].append(ref[0].strip())
                        }
                    }
                }
                cve.pop('references-CVE_MITRE', None)
            }
            if 'references-CVE_DETAILS' in cve{
                refs_details=cve['references-CVE_DETAILS']
                for ref in refs_details{
                    ref_url=ref.strip()
                    if ref_url not in cve['References']{
                        cve['References'].append(ref_url)
                        cve['References-class'].append('MISC')
                    }
                }
                cve.pop('references-CVE_DETAILS', None)
            }
            for i in range(len(cve['References-class'])){
                if len(cve['References-class'][i])>1 and 'MISC' in cve['References-class'][i]{
                    cve['References-class'][i].remove('MISC')
                }
            }
            # References

            # Description
            cve['Description']=[]
            if 'description-CVE_MITRE' in cve{
                cve_description=cve['description-CVE_MITRE'].strip()
                if cve_description not in cve['Description']{
                    cve['Description'].append(cve_description)
                }
            }
            if 'description-CVE_NVD' in cve{
                for desc in cve['description-CVE_NVD']['description_data']{
                    cve_description=desc['value'].strip()
                    if cve_description not in cve['Description']{
                        cve['Description'].append(cve_description)
                    }
                }
            }
            if 'description-CVE_DETAILS' in cve{
                cve_description=cve['description-CVE_DETAILS'].strip().replace('&quot;','\"')
                if cve_description not in cve['Description']{
                    cve['Description'].append(cve_description)
                }
            }
            if len(cve['Description'])>0{
                cve['Description']='\n'.join(cve['Description'])
                if '** RESERVED **' in cve['Description']{
                    cve['Status']='RESERVED'
                    cve.pop('Description', None)
                }
            }
            # Description
            
            # Phase
            if 'Phase' in cve{
                result=re.match(r'([a-zA-Z]*) \(([0-9]{8})\)', cve['Phase'])
                if result{
                    cve['Phase']=result.group(1)
                    cve['{} Date'.format(result.group(1))]=Utils.changeStrDateFormat(result.group(2),'%Y%m%d','%d/%m/%Y')
                }else{
                    cve['Phase']=cve['Phase'].strip()
                }
            }
            # Phase

            if 'Votes' in cve{
                cve.pop('Votes', None)
            }


            # problemtype AND cwe
            # TODO
            # problemtype AND cwe

            # check - TODO fix
            for k,v in cve.items(){
                if type(v) not in (int,str) and k!='_id'{
                    if type(v) is list{
                        for el in v{
                            if type(el) not in (int,str){
                                raise Exception('Non-flat field on {} inside list {}: type:{} v:{}'.format(cve['cve'],k,type(el),el))
                            }
                        }
                    }else{
                        raise Exception('Non-flat field on {}: type:{} k:{} v:{}'.format(cve['cve'],type(v),k,v))
                    }
                }
            }
            if update_callback { update_callback() }
            self.mongo.insertOneOnDB(self.mongo.getProcessedDB(),cve,'flat_cve','cve',verbose=False)
        }


        self.logger.info('Runned \"Flattern and Simplify\" on CVEs Data...OK')
    }

    def flatternAndSimplifyOval(self,update_callback=None){
        pass # TODO
    }

    def flatternAndSimplifyCapec(self,update_callback=None){
        pass # TODO
    }

    def flatternAndSimplifyCwe(self,update_callback=None){
        pass # TODO
    }

    def filterExploits(self,update_callback=None){
        pass # TODO
    }   

    def transformCve(self,update_callback=None){
        pass # TODO
    }

    def transformOval(self,update_callback=None){
        pass # TODO
    }

    def transformCapec(self,update_callback=None){
        pass # TODO
    }

    def transformCwe(self,update_callback=None){
        pass # TODO
    }

    def transformExploits(self,update_callback=None){
        pass # TODO
    }
    
    def enrichData(self,update_callback=None){
        # TODO include CWE, CAPEC, OVAL, exploit into CVE DATA
        # (insert using data on CVEs)
        # (insert using data on the other sources, search if the given exploit already fulfilled and if data is complete)
        pass
    }

    def analyzeFullDataset(self,update_callback=None){
        # TODO check the coverage of each feature to decide later which ones to exclude
        # return list of key and its coverage
        pass
    }

    def filterAndNormalizeFullDataset(self,update_callback=None){
        # TODO filter bad features
        pass
    }

    def runPipeline(self,update_callback=None){
        self.mergeCve(update_callback=update_callback)
        self.flatternAndSimplifyCve(update_callback=update_callback)
        self.flatternAndSimplifyOval(update_callback=update_callback)
        self.flatternAndSimplifyCapec(update_callback=update_callback)
        self.flatternAndSimplifyCwe(update_callback=update_callback)
        self.filterExploits(update_callback=update_callback)
        self.transformCve(update_callback=update_callback)
        self.transformOval(update_callback=update_callback)
        self.transformCapec(update_callback=update_callback)
        self.transformCwe(update_callback=update_callback)
        self.transformExploits(update_callback=update_callback)
        self.enrichData(update_callback=update_callback)
        self.analyzeFullDataset(update_callback=update_callback)
        self.filterAndNormalizeFullDataset(update_callback=update_callback)
    }

}