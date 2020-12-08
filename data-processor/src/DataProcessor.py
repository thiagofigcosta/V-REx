#!/bin/python
# -*- coding: utf-8 -*-

from Utils import Utils
import re
import time

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
        iter_count=0
        data_size=0
        total_iters=len(self.references['cve'])
        self.logger.info('Running \"Merge\" on CVEs Data...')
        lock=self.mongo.getLock(self.mongo.getProcessedDB(),'merged_cve')
        while self.mongo.checkIfCollectionIsLocked(lock=lock){
            time.sleep(1)
        }
        lock.acquire()
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
            if update_callback { update_callback() }
            iter_count+=1
            if len(merged_entry)>0{
                self.mongo.insertOneOnDB(self.mongo.getProcessedDB(),merged_entry,'merged_cve','cve',verbose=False,ignore_lock=True)
                data_size+=Utils.sizeof(merged_entry)
            }
            if iter_count%verbose_frequency==0{
                lock.refresh()
                self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
            }
        }
        self.logger.info('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
        lock.release()
        self.logger.info('Runned \"Merge\" on CVEs Data...OK')
    }

    def flatternAndSimplifyCve(self,update_callback=None){
        # cve - OK
        # Status - OK
        # description-CVE_MITRE - OK
        # references-CVE_MITRE - OK
        # Phase - OK
        # Votes - OK
        # problemtype - OK
        # references-CVE_NVD - OK
        # description-CVE_NVD - OK
        # configurations - OK
        # impact - OK
        # publishedDate - OK
        # lastModifiedDate - OK
        # cvss score - OK
        # confidentiality imp. - OK
        # integrity imp. - OK
        # availability imp. - OK
        # complexity - OK
        # authentication - OK
        # gained acc. - OK
        # vul. type - OK
        # publish date - OK
        # last mod date - OK
        # description-CVE_DETAILS - OK
        # prod. affected - OK
        # versions affected - OK
        # references-CVE_DETAILS - OK
        # cwe - OK
        # metasploitable - OK
        # Comments - OK
        self.logger.info('Running \"Flattern and Simplify\" on CVEs Data...')
        merged_data=self.mongo.findAllOnDB(self.mongo.getProcessedDB(),'merged_cve')
        verbose_frequency=1333
        iter_count=0
        data_size=0
        total_iters=merged_data.count()
        lock=self.mongo.getLock(self.mongo.getProcessedDB(),'flat_cve')
        while self.mongo.checkIfCollectionIsLocked(lock=lock){
            time.sleep(1)
        }
        lock.acquire()
        for cve in merged_data{
            # References
            cve['References']=[]
            cve['References_class']=[]
            if 'references-CVE_NVD' in cve{
                refs_mitre=cve['references-CVE_NVD']
                for ref in refs_mitre{
                    ref_url=ref['url'].strip()
                    if ref_url not in cve['References']{
                        cve['References'].append(ref_url)
                        if 'tags' in ref{
                            cve['References_class'].append(ref['tags'])
                        }else{
                            cve['References_class'].append(['URL'])
                        }
                    }else{
                        ref_idx=cve['References'].index(ref_url)
                        for tag in ref['tags']{
                            if tag.strip() not in cve['References_class'][ref_idx]{
                                cve['References_class'][ref_idx].append(tag.strip())
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
                    if len(ref)>1{
                        ref_url=ref[1].strip()
                        if ref_url not in cve['References']{
                            cve['References'].append(ref_url)
                            cve['References_class'].append([ref[0].strip()])
                        }else{
                            ref_idx=cve['References'].index(ref_url)
                            if ref[0].strip() not in cve['References_class'][ref_idx]{
                                cve['References_class'][ref_idx].append(ref[0].strip())
                            }
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
                        cve['References_class'].append(['URL'])
                    }
                }
                cve.pop('references-CVE_DETAILS', None)
            }
            for i in range(len(cve['References_class'])){
                if len(cve['References_class'][i])>1 and 'URL' in cve['References_class'][i]{
                    cve['References_class'][i].remove('URL')
                }
            }
            if len(cve['References'])==0{
                cve.pop('References', None)
                cve.pop('References_class', None)
            }
            # References

            # Description
            cve['Description']=[]
            if 'description-CVE_MITRE' in cve{
                cve_description=cve['description-CVE_MITRE'].strip()
                if cve_description not in cve['Description']{
                    cve['Description'].append(cve_description)
                }
                cve.pop('description-CVE_MITRE', None)
            }
            if 'description-CVE_NVD' in cve{
                for desc in cve['description-CVE_NVD']['description_data']{
                    cve_description=desc['value'].strip()
                    if cve_description not in cve['Description']{
                        cve['Description'].append(cve_description)
                    }
                }
                cve.pop('description-CVE_NVD', None)
            }
            if 'description-CVE_DETAILS' in cve{
                cve_description=cve['description-CVE_DETAILS'].strip().replace('&quot;','\"')
                if cve_description not in cve['Description']{
                    cve['Description'].append(cve_description)
                }
                cve.pop('description-CVE_DETAILS', None)
            }
            if len(cve['Description'])>0{
                cve['Description']='\n'.join(cve['Description'])
                if '** RESERVED **' in cve['Description']{
                    cve['Status']='RESERVED'
                    cve.pop('Description', None)
                }
            }else{
                cve.pop('Description', None)
            }
            # Description
            
            # Phase
            if 'Phase' in cve{
                result=re.match(r'([a-zA-Z]*) \(([0-9]{8})\)', cve['Phase'])
                if result{
                    cve['Phase']=result.group(1)
                    cve['{}Date'.format(result.group(1).lower())]=Utils.changeStrDateFormat(result.group(2),'%Y%m%d','%d/%m/%Y')
                }else{
                    cve['Phase']=cve['Phase'].strip()
                }
            }
            # Phase

            if 'Votes' in cve{
                cve.pop('Votes', None)
            }

            # problemtype AND cwe
            cve['CWEs']=[]
            if 'problemtype' in cve{
                cwes=cve['problemtype']['problemtype_data']
                for cwe in cwes{
                    cwes2=cwe['description']
                    for cwe in cwes2{
                        cwe=cwe['value'].split('-')[1]
                        if cwe.isdecimal(){
                            cve['CWEs'].append(cwe)
                        }
                    }
                }
                cve.pop('problemtype', None)
            }
            if 'cwe' in cve{
                cwe=cve['cwe']
                if cwe.isdecimal() and cwe not in cve['CWEs']{
                    cve['CWEs'].append(cwe)
                }
                cve.pop('cwe', None)
            }
            if len(cve['CWEs'])==0{
                cve.pop('CWEs', None)
            }
            # problemtype AND cwe

            # configurations and prod. affected and versions affected
            cve['vendors']=set()
            cve['products']=set()
            if 'configurations' in cve{
                cve['CPEs_vulnerable']=[]
                cve['CPEs_non_vulnerable']=[]
                for conf in cve['configurations']{
                    if 'cpe_match' in conf{
                        for cpe in conf['cpe_match']{
                            if cpe['vulnerable']{
                                cve['CPEs_vulnerable'].append(cpe['cpe23Uri'])
                                uncompressed_cpe=cpe['cpe23Uri'].split(':')
                                cve['vendors'].add(uncompressed_cpe[3].lower())
                                cve['products'].add(uncompressed_cpe[4].lower())
                            }else{
                                cve['CPEs_non_vulnerable'].append(cpe['cpe23Uri'])
                            }
                        }
                    }
                    if 'children' in conf{
                        for children in conf['children']{
                            if 'cpe_match' in children{
                                for cpe in children['cpe_match']{
                                    if cpe['vulnerable']{
                                        cve['CPEs_vulnerable'].append(cpe['cpe23Uri'])
                                        uncompressed_cpe=cpe['cpe23Uri'].split(':')
                                        cve['vendors'].add(uncompressed_cpe[3].lower())
                                        cve['products'].add(uncompressed_cpe[4].lower())
                                    }else{
                                        cve['CPEs_non_vulnerable'].append(cpe['cpe23Uri'])
                                    }
                                }
                            }
                        }
                    }
                }
                cve.pop('configurations', None)
                if len(cve['CPEs_non_vulnerable'])==0{
                    cve.pop('CPEs_non_vulnerable', None)
                }
                if len(cve['CPEs_vulnerable'])==0{
                    cve.pop('CPEs_vulnerable', None)
                }
            }

            if 'versions affected' in cve {
                cve['AffectedVersionsCount']=0
                for prod in cve['versions affected']{
                    cve['vendors'].add(prod['Vendor'].lower())
                    if 'Product' in prod{
                        cve['products'].add(prod['Product'].lower())
                    }
                    cve['AffectedVersionsCount']+=int(prod['Vulnerable Versions'])
                }
                cve.pop('versions affected', None)
            }
            if 'prod. affected' in cve {
                cve.pop('prod. affected', None)
            }
            cve['vendors']=list(cve['vendors'])
            if '*' in cve['vendors']{
                cve['vendors'].remove('*')
            }
            cve['products']=list(cve['products'])
            if '*' in cve['products']{
                cve['products'].remove('*')
            }
            if len(cve['vendors'])==0{
                cve.pop('vendors', None)
            }
            if len(cve['products'])==0{
                cve.pop('products', None)
            }
            # configurations and prod. affected and versions affected

            # impact
            if 'impact' in cve{
                if 'baseMetricV2' in cve['impact']{
                    cve['CVSS_version']=cve['impact']['baseMetricV2']['cvssV2']['version']
                    cve['CVSS_score']=cve['impact']['baseMetricV2']['cvssV2']['baseScore']
                    cve['CVSS_AV']=cve['impact']['baseMetricV2']['cvssV2']['accessVector']
                    cve['CVSS_AC']=cve['impact']['baseMetricV2']['cvssV2']['accessComplexity']
                    cve['CVSS_AuPR']=cve['impact']['baseMetricV2']['cvssV2']['authentication']
                    cve['CVSS_C']=cve['impact']['baseMetricV2']['cvssV2']['confidentialityImpact']
                    cve['CVSS_I']=cve['impact']['baseMetricV2']['cvssV2']['integrityImpact']
                    cve['CVSS_A']=cve['impact']['baseMetricV2']['cvssV2']['availabilityImpact']
                    cve['CVSS_exploitabilityScore']=cve['impact']['baseMetricV2']['exploitabilityScore']
                    cve['CVSS_impactScore']=cve['impact']['baseMetricV2']['impactScore']
                }
                if 'baseMetricV3' in cve['impact']{
                    cve['CVSS_version']=cve['impact']['baseMetricV3']['cvssV3']['version']
                    cve['CVSS_score']=cve['impact']['baseMetricV3']['cvssV3']['baseScore']
                    cve['CVSS_AV']=cve['impact']['baseMetricV3']['cvssV3']['attackVector']
                    cve['CVSS_AC']=cve['impact']['baseMetricV3']['cvssV3']['attackComplexity']
                    cve['CVSS_AuPR']=cve['impact']['baseMetricV3']['cvssV3']['privilegesRequired']
                    cve['CVSS_UI']=cve['impact']['baseMetricV3']['cvssV3']['userInteraction']
                    cve['CVSS_S']=cve['impact']['baseMetricV3']['cvssV3']['scope']
                    cve['CVSS_C']=cve['impact']['baseMetricV3']['cvssV3']['confidentialityImpact']
                    cve['CVSS_I']=cve['impact']['baseMetricV3']['cvssV3']['integrityImpact']
                    cve['CVSS_A']=cve['impact']['baseMetricV3']['cvssV3']['availabilityImpact']
                    cve['CVSS_exploitabilityScore']=cve['impact']['baseMetricV3']['exploitabilityScore']
                    cve['CVSS_impactScore']=cve['impact']['baseMetricV3']['impactScore']
                }
                cve.pop('impact', None)
            }
            # impact

            if 'publishedDate' in cve{
                cve['publishedDate']=Utils.changeStrDateFormat(cve['publishedDate'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
            }

            if 'lastModifiedDate' in cve{
                cve['lastModifiedDate']=Utils.changeStrDateFormat(cve['lastModifiedDate'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
            }

            if 'cvss score' in cve{
                if 'CVSS_score' not in cve{
                    cve['CVSS_score']=cve['cvss score']
                }
                cve.pop('cvss score', None)
            }

            if 'confidentiality imp.' in cve{
                if 'CVSS_C' not in cve{
                    cve['CVSS_C']=cve['confidentiality imp.'].upper()
                }
                cve.pop('confidentiality imp.', None)
            }

            if 'integrity imp.' in cve{
                if 'CVSS_I' not in cve{
                    cve['CVSS_I']=cve['integrity imp.'].upper()
                }
                cve.pop('integrity imp.', None)
            }

            if 'availability imp.' in cve{
                if 'CVSS_A' not in cve{
                    cve['CVSS_A']=cve['availability imp.'].upper()
                }
                cve.pop('availability imp.', None)
            }

            if 'complexity' in cve{
                if 'CVSS_AC' not in cve{
                    cve['CVSS_AC']=cve['complexity'].upper()
                }
                cve.pop('complexity', None)
            }

            if 'authentication' in cve{
                if 'CVSS_AuPR' not in cve{
                    cve['CVSS_AuPR']=cve['authentication'].replace('Not required','NONE').upper()
                }
                cve.pop('authentication', None)
            }

            if 'vul. type' in cve{
                vul=cve['vul. type'].split('-',1)
                if len(vul)>1{
                    cve['Type']=vul[1].strip()
                }else{
                    cve['Type']=vul[0].strip()
                }
                cve.pop('vul. type', None)
            }

            if 'publish date' in cve{
                date=Utils.changeStrDateFormat(cve['publish date'],'%Y-%m-%d','%d/%m/%Y')
                if 'publishedDate' not in cve{
                    cve['publishedDate']=date
                }else{
                    if Utils.isFirstStrDateOldest(date,cve['publishedDate'],'%d/%m/%Y'){ # oldest
                        cve['publishedDate']=date
                    }
                }
                cve.pop('publish date', None)
            }

            if 'last mod date' in cve{
                date=Utils.changeStrDateFormat(cve['last mod date'],'%Y-%m-%d','%d/%m/%Y')
                if 'lastModifiedDate' not in cve{
                    cve['lastModifiedDate']=date
                }else{
                    if Utils.isFirstStrDateOldest(cve['lastModifiedDate'],date,'%d/%m/%Y'){ # newest
                        cve['lastModifiedDate']=date
                    }
                }
                cve.pop('last mod date', None)
            }

            if 'gained acc.' in cve{
                cve.pop('gained acc.', None)
            }

            if 'metasploitable' in cve{
                modules={}
                for module in cve['metasploitable']{
                    for k,v in module.items(){
                        result=re.match(r'Module type\s*:\s*([A-Za-z]*)', v)
                        mod_type='other'
                        if result{
                            mod_type=result.group(1)
                        }
                        if mod_type not in modules{
                            modules[mod_type]=1
                        }else{
                            modules[mod_type]+=1
                        }
                    }
                }
                cve.pop('metasploitable', None)
                cve['weaponized_modules_types']=[]
                cve['weaponized_modules_count']=[]
                for k,v in modules.items(){
                    cve['weaponized_modules_types'].append(k)
                    cve['weaponized_modules_count'].append(v)
                }
            }

            if 'Comments' in cve{
                cve['Comments']=len(cve['Comments'].split('|'))
            }

            for k,v in cve.items(){
                if type(v) not in (int,str,float) and k not in ('_id','References_class'){
                    if type(v) is list{
                        for el in v{
                            if type(el) not in (int,str,float){
                                raise Exception('Non-flat field on {} inside list {}: type:{} v:{}'.format(cve['cve'],k,type(el),el))
                            }
                        }
                    }else{
                        raise Exception('Non-flat field on {}: type:{} k:{} v:{}'.format(cve['cve'],type(v),k,v))
                    }
                }
            }
            if update_callback { update_callback() }
            self.mongo.insertOneOnDB(self.mongo.getProcessedDB(),cve,'flat_cve','cve',verbose=False,ignore_lock=True)
            data_size+=Utils.sizeof(cve)
            iter_count+=1
            if iter_count%verbose_frequency==0{
                lock.refresh()
                self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
            }
        }
        self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
        lock.release()
        self.logger.info('Runned \"Flattern and Simplify\" on CVEs Data...OK')
    }

    def flatternAndSimplifyOval(self,update_callback=None){
        self.logger.info('Running \"Flattern and Simplify\" on OVAL Data...')
        oval_data=self.mongo.findAllOnDB(self.mongo.getRawDB(),'OVAL')
        verbose_frequency=1333
        iter_count=0
        data_size=0
        total_iters=oval_data.count()
        lock=self.mongo.getLock(self.mongo.getProcessedDB(),'flat_oval')
        while self.mongo.checkIfCollectionIsLocked(lock=lock){
            time.sleep(1)
        }
        lock.acquire()
        for oval in oval_data{
            oval_parsed={}
            append=False
            if 'reference' in oval{
                if type(oval['reference']) is list{
                    for ref in oval['reference']{
                        if ref['source']=='CVE'{
                            append=True
                            oval_parsed['CVE']=ref['ref_id']
                            break
                        }
                    }
                }else{
                    if oval['reference']['source']=='CVE'{
                        append=True
                        oval_parsed['CVE']=oval['reference']['ref_id']
                    }
                }
            }
            if append{
                oval_parsed['oval']=oval['oval'].split(':')[-1]
                oval_parsed['type']=oval['class']
                if 'oval_repository' in oval{
                    if 'dates' in oval['oval_repository']{
                        if 'submitted' in oval['oval_repository']['dates']{
                            oval_parsed['submittedDate']=Utils.changeStrDateFormat(oval['oval_repository']['dates']['submitted']['date'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
                        }
                        if 'modified' in oval['oval_repository']['dates']{
                            if type(oval['oval_repository']['dates']['modified']) is list{
                                oval_parsed['modifiedDate']=Utils.changeStrDateFormat(oval['oval_repository']['dates']['modified'][0]['date'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
                                for entry in oval['oval_repository']['dates']['modified']{
                                    date=Utils.changeStrDateFormat(entry['date'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
                                    if Utils.isFirstStrDateOldest(oval_parsed['modifiedDate'],date,'%d/%m/%Y'){ # newest
                                        oval_parsed['modifiedDate']=date
                                    }
                                }
                            }else{
                                oval_parsed['modifiedDate']=Utils.changeStrDateFormat(oval['oval_repository']['dates']['modified']['date'].split('T')[0],'%Y-%m-%d','%d/%m/%Y')
                            }
                        }
                    }
                }
                self.mongo.insertOneOnDB(self.mongo.getProcessedDB(),oval_parsed,'flat_oval','oval',verbose=False,ignore_lock=True)
                data_size+=Utils.sizeof(oval_parsed)
            }
            if update_callback { update_callback() }
            iter_count+=1
            if iter_count%verbose_frequency==0{
                lock.refresh()
                self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
            }
        }
        self.logger.verbose('Percentage done {:.2f}% - Total data size: {}'.format((float(iter_count)/total_iters*100),Utils.bytesToHumanReadable(data_size)))
        lock.release()
        self.logger.info('Runned \"Flattern and Simplify\" on OVAL Data...OK')
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