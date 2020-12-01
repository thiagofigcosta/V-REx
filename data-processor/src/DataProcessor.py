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

    def flatternAndSimplifyCve(self,update_callback=None){
        pass # TODO
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