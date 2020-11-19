#!/bin/python
# -*- coding: utf-8 -*-

from Utils import Utils

class DataProcessor(object){
    TMP_FOLDER='tmp/processor/'

    def __init__(self, mongo, logger){
        self.logger=logger
		self.mongo=mongo
        self.references=self.mongo.loadReferences()
    }

    def mergeCveData(self,update_callback=None){
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
        self.logger.info('Merging and inserting CVEs Data...')
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
                if update_callback {
                    update_callback()
                }
                if verbose_frequency==frequency_count{
                    self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((iter_count/total_iters*100),Utils.bytesToHumanReadable(data_size)))
                    frequency_count=0
                }
            }
            iter_count+=1
            frequency_count+=1
        }
        self.logger.info('Percentage done {:.2f}% - Total data size: {}'.format((iter_count/total_iters*100),Utils.bytesToHumanReadable(data_size)))
        self.logger.info('Merged and inserted CVEs Data...OK')
    }

    def transformCveData(self,update_callback=None){
        # TODO flattern and prepare data (transform into features)
        pass
    }

    def transformExtraData(self,update_callback=None){
        # TODO flattern and prepare data (transform into features)
        # TODO CWE
        # TODO CAPEC
        # TODO OVAL
        # TODO EXPLOIT
        pass
    }
    
    def enrichData(self,update_callback=None){
        # TODO include CWE, CAPEC, OVAL, exploit into CVE DATA
        # (insert using data on CVEs)
        # (insert using data on the other sources, search if the given exploit already fulfilled and if data is complete)
        pass
    }

    def analyzeData(self,update_callback=None){
        # TODO check the coverage of each feature to decide later which ones to exclude
        # return list of key and its coverage
        pass
    }

    def filterData(self,analyze_result,update_callback=None){
        # TODO filter bad features
        pass
    }

    def runPipeline(self,update_callback=None){
        self.mergeCveData(update_callback=update_callback)
        self.transformCveData(update_callback=update_callback)
        self.transformExtraData(update_callback=update_callback)
        self.enrichData(update_callback=update_callback)
        analyze_result=self.analyzeData(update_callback=update_callback)
        self.filterData(analyze_result,update_callback=update_callback)
    }

    # cve
    # Status
    # description-CVE_MITRE
    # references-CVE_MITRE
    # Phase
    # Votes
    # problemtype
    # references-CVE_NVD
    # description-CVE_NVD
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
    # description-CVE_DETAILS
    # prod. affected
    # versions affected
    # references-CVE_DETAILS
    # cwe
    # metasploitable
    # Comments
}