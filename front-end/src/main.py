#!/bin/python

import sys, getopt
from Utils import Utils
from Logger import Logger
from MongoDB import MongoDB

TMP_FOLDER='tmp/front/'

ITERATIVE=False

Utils.createFolderIfNotExists(TMP_FOLDER)
LOGGER=Logger(TMP_FOLDER,name='front')
Utils(TMP_FOLDER,LOGGER)

def inputNumber(is_float=False){
    out=0
    converted=False
    while not converted{
        try{
            if is_float{
                out=float(input())
            }else{
                out=int(input())
            }
            converted=True
        }except ValueError{
            if not is_float{
                print('ERROR. Not an integer, type again: ')
            }else{
                print('ERROR. Not a float number, type again: ')
            } 
        }
    }
    return out
}

def main(argv){
    HELP_STR='main.py [-h]\n\t[--check-jobs]\n\t[--create-genetic-env]\n\t[--list-genetic-envs]\n\t[--rm-genetic-env <env name>]\n\t[--get-queue-names]\n\t[--get-all-db-names]\n\t[-q | --quit]\n\t[--run-processor-pipeline]\n\t[--run-merge-cve]\n\t[--run-flattern-and-simplify-all]\n\t[--run-flattern-and-simplify [cve|oval|capec|cwe]]\n\t[--run-filter-exploits]\n\t[--run-transform-all]\n\t[--run-transform [cve|oval|capec|cwe|exploits]]\n\t[--run-enrich]\n\t[--run-analyze]\n\t[--run-filter-and-normalize]\n\t[--download-source <source ID>]\n\t[--download-all-sources]\n\t[--empty-queue <queue name>]\n\t[--empty-all-queues]\n\t[--dump-db <db name>#<folder path to export> | --dump-db <db name> {saves on default tmp folder} \n\t\te.g. --dump-db queue#/home/thiago/Desktop]\n\t[--restore-db <file path to import>#<db name> | --restore-db <file path to import> {saves db under file name} \n\t\te.g. --restore-db /home/thiago/Desktop/queue.zip#restored_queue]\n\t[--keep-alive-as-zombie]'
    args=[]
    zombie=False
    global ITERATIVE
    to_run=[]
    try{ 
        opts, args = getopt.getopt(argv,"hq",["keep-alive-as-zombie","download-source=","download-all-sources","check-jobs","quit","get-queue-names","empty-queue=","empty-all-queues","get-all-db-names","dump-db=","restore-db=","run-processor-pipeline","run-flattern-and-simplify-all","run-flattern-and-simplify=","run-filter-exploits","run-transform-all","run-transform=","run-enrich","run-analyze","run-filter-and-normalize","run-merge-cve","create-genetic-env","list-genetic-envs","rm-genetic-env="])
    }except getopt.GetoptError{
        print (HELP_STR)
        if not ITERATIVE {
            sys.exit(2)
        }
    }
    try{
        for opt, arg in opts{ 
            if opt == '-h'{
                print (HELP_STR)
            }elif opt in ('-q','--quit'){
                exit()
            }elif opt == "--keep-alive-as-zombie"{
                zombie=True
            }elif opt == "--dump-db"{
                splited_arg=arg.split('#')
                if len(splited_arg)>0 and len(splited_arg)<=2{
                    path_to_export=TMP_FOLDER
                    if len(splited_arg)>1{
                        path_to_export=splited_arg[1]
                    }
                    mongo.dumpDB(mongo.getDB(splited_arg[0]),path_to_export)
                }else{
                    LOGGER.error('Invalid argument, type the db_name or "db_name#path": {}'.format(arg))
                }
            }elif opt == "--list-genetic-envs"{
                LOGGER.info('Getting genetic environments...')
                for env in mongo.findAllOnDB(mongo.getDB('genetic_db'),'environments',wait_unlock=False){
                    LOGGER.clean('Name: {}'.format(env['name']))
                    LOGGER.clean('Submitted At: {}'.format(env['submitted_at']))
                    LOGGER.clean('Space Search:')
                    LOGGER.clean('\tInt:')
                    for el in env['space_search']['int'] {
                        for k,v in el.items(){
                            LOGGER.clean('\t\t{}: {}'.format(k,str(v)))
                        }
                    }
                    LOGGER.clean('\tFloat:')
                    for el in env['space_search']['float'] {
                        for k,v in el.items(){
                            LOGGER.clean('\t\t{}: {}'.format(k,str(v)))
                        }
                    }
                    LOGGER.clean('\n')
                }
                LOGGER.info('Gotten genetic environments...OK')
            }elif opt == "--create-genetic-env"{
                print('Now type the minimum and maximums for each item of the Smart Neural Search Space...')
                print('Enter the genetic environment name: ', end = '')
                gen_name=input().strip()
                submitted_at=Utils.getTodayDate('%d/%m/%Y %H:%M:%S')
                print('Enter the amount of layers: min: ')
                amount_of_layers_min=inputNumber()
                print("Max: ")
                amount_of_layers_max=inputNumber()
                print('Enter the epochs: min: ')
                epochs_min=inputNumber()
                print("Max: ")
                epochs_max=inputNumber()
                print('Enter the alpha: min: ')
                alpha_min=inputNumber(is_float=True)
                print("Max: ")
                alpha_max=inputNumber(is_float=True)
                print('Enter the batch size: min: ')
                batch_size_min=inputNumber()
                print("Max: ")
                batch_size_max=inputNumber()
                print('Enter the layer sizes: min: ')
                layer_size_min=inputNumber()
                print("Max: ")
                layer_size_max=inputNumber()
                print('Enter the range pow: min: ')
                range_pow_min=inputNumber()
                print("Max: ")
                range_pow_max=inputNumber()
                print('Enter the K values: min: ')
                k_min=inputNumber()
                print("Max: ")
                k_max=inputNumber()
                print('Enter the L values: min: ')
                l_min=inputNumber()
                print("Max: ")
                l_max=inputNumber()
                print('Enter the sparcity: min: ')
                sparcity_min=inputNumber(is_float=True)
                print("Max: ")
                sparcity_max=inputNumber(is_float=True)
                print('Enter the activation functions (0-2):')
                print('\t0 - ReLU')
                print('\t1 - Softmax')
                print('\t2 - Sigmoid')
                print('min: ')
                activation_min=inputNumber()
                print("Max: ")
                activation_max=inputNumber()
                space_search={'name':gen_name,'submitted_at':submitted_at,'space_search':{'int':[{'amount_of_layers':{'min':amount_of_layers_min,'max':amount_of_layers_max}},{'epochs':{'min':epochs_min,'max':epochs_max}},{'batch_size':{'min':batch_size_min,'max':batch_size_max}},{'layer_sizes':{'min':layer_size_min,'max':layer_size_max}},{'range_pow':{'min':range_pow_min,'max':range_pow_max}},{'K':{'min':k_min,'max':k_max}},{'L':{'min':l_min,'max':l_max}},{'activation_functions':{'min':activation_min,'max':activation_max}}],'float':[{'sparcity':{'min':sparcity_min,'max':sparcity_max}},{'alpha':{'min':alpha_min,'max':alpha_max}}]}}
                LOGGER.info('Writting environment on genetic_db...')
                mongo.insertOneOnDB(mongo.getDB('genetic_db'),space_search,'environments',index='name',ignore_lock=True)
                LOGGER.info('Wrote on environment on genetic_db...OK')
            }elif opt == "--rm-genetic-env"{
                arg=arg.strip()
                LOGGER.info('Removing {} from genetic environments...'.format(arg))
                query={'name':arg}
                mongo.rmOneFromDB(mongo.getDB('genetic_db'),'environments',query=query)
                # mongo.insertOnProcessorQueue('Run Pipeline')
                LOGGER.info('Removed {} from genetic environments...OK'.format(arg))
            }elif opt == "--run-processor-pipeline"{
                LOGGER.info('Writting on Processor to Run the entire Pipeline...')
                mongo.insertOnProcessorQueue('Run Pipeline')
                LOGGER.info('Wrote on Processor to Run the entire Pipeline...OK')
            }elif opt == "--run-merge-cve"{ 
                LOGGER.info('Writting on Processor to Run Merge CVE step...')
                mongo.insertOnProcessorQueue('Merge')
                LOGGER.info('Wrote on Processor to  Run Merge CVE step...OK')
            }elif opt == "--run-flattern-and-simplify-all"{
                LOGGER.info('Writting on Processor to Flattern and Simplify all types of data...')
                job_args={'type':'CVE'}
                mongo.insertOnProcessorQueue('Flattern and Simplify',job_args)
                job_args={'type':'OVAL'}
                mongo.insertOnProcessorQueue('Flattern and Simplify',job_args)
                job_args={'type':'CAPEC'}
                mongo.insertOnProcessorQueue('Flattern and Simplify',job_args)
                job_args={'type':'CWE'}
                mongo.insertOnProcessorQueue('Flattern and Simplify',job_args)
                LOGGER.info('Wrote on Processor to Flattern and Simplify all types of data...OK')
            }elif opt == "--run-flattern-and-simplify"{
                if arg.lower() in ('cve','oval','capec','cwe'){
                    job_args={'type':arg.upper()}
                    LOGGER.info('Writting on Processor to Flattern and Simplify {}...'.format(arg))
                    mongo.insertOnProcessorQueue('Flattern and Simplify',job_args)
                    LOGGER.info('Wrote on Processor to Flattern and Simplify {}...'.format(arg))
                }else{
                    LOGGER.error('Invalid argument, arg must be cve, oval, capec or cwe')
                }
            }elif opt == "--run-filter-exploits"{
                LOGGER.info('Writting on Processor to Filter Exploits...')
                mongo.insertOnProcessorQueue('Filter Exploits')
                LOGGER.info('Wrote on Processor to Filter Exploits...OK')
            }elif opt == "--run-transform-all"{
                LOGGER.info('Writting on Processor to Transform all types of data...')
                job_args={'type':'CVE'}
                mongo.insertOnProcessorQueue('Transform',job_args)
                job_args={'type':'OVAL'}
                mongo.insertOnProcessorQueue('Transform',job_args)
                job_args={'type':'CAPEC'}
                mongo.insertOnProcessorQueue('Transform',job_args)
                job_args={'type':'CWE'}
                mongo.insertOnProcessorQueue('Transform',job_args)
                job_args={'type':'EXPLOITS'}
                mongo.insertOnProcessorQueue('Transform',job_args)
                LOGGER.info('Wrote on Processor to Transform all types of data...OK')
            }elif opt == "--run-transform"{
                if arg.lower() in ('cve','oval','capec','cwe','exploits'){
                    job_args={'type':arg.upper()}
                    LOGGER.info('Writting on Processor to Transform {}...'.format(arg))
                    mongo.insertOnProcessorQueue('Transform',job_args)
                    LOGGER.info('Wrote on Processor to Transform {}...'.format(arg))
                }else{
                    LOGGER.error('Invalid argument, arg must be cve, oval, capec or cwe')
                }
            }elif opt == "--run-enrich"{
                LOGGER.info('Writting on Processor to Enrich Data...')
                mongo.insertOnProcessorQueue('Enrich')
                LOGGER.info('Wrote on Processor to Enrich Data...OK')
            }elif opt == "--run-analyze"{
                LOGGER.info('Writting on Processor to Analyze Data...')
                mongo.insertOnProcessorQueue('Analyze')
                LOGGER.info('Wrote on Processor to Analyze Data...OK')
            }elif opt == "--run-filter-and-normalize"{
                LOGGER.info('Writting on Processor to Filter and Normalize Data...')
                mongo.insertOnProcessorQueue('Filter and Normalize')
                LOGGER.info('Wrote on Processor to Filter and Normalize Data...OK')
            }elif opt == "--restore-db"{
                splited_arg=arg.split('#')
                if len(splited_arg)>0 and len(splited_arg)<=2{
                    db_name=None
                    if len(splited_arg)>1{
                        db_name=splited_arg[1]
                    }
                    mongo.restoreDB(splited_arg[0],db_name)
                }else{
                    LOGGER.error('Invalid argument, type the compressed_file_path or "compressed_file_path#db_name": {}'.format(arg))
                }
            }elif opt == "--empty-queue"{
                LOGGER.info('Erasing queue {}...'.format(arg))
                mongo.clearQueue(arg)
                LOGGER.info('Erased queue {}...OK'.format(arg))
            }elif opt == "--empty-all-queues"{
                LOGGER.info('Erasing all queues...')
                for queue in mongo.getQueueNames(){
                    LOGGER.info('Erasing queue {}...'.format(queue))
                    mongo.clearQueue(queue)
                    LOGGER.info('Erased queue {}...OK'.format(queue))
                }
                LOGGER.info('Erased all queues...OK')
            }elif opt == "--get-queue-names"{
                LOGGER.info('Getting queue names...')
                for queue in mongo.getQueueNames(){
                    LOGGER.clean('\t{}'.format(queue))
                }
                LOGGER.info('Gotten queue names...OK')
            }elif opt == "--get-all-db-names"{
                LOGGER.info('Getting db names...')
                for db in mongo.getAllDbNames(){
                    LOGGER.clean('\t{}'.format(db))
                }
                LOGGER.info('Gotten db names...OK')
            }elif opt == "--download-source"{
                LOGGER.info('Writting on Crawler to Download {}...'.format(arg))
                job_args={'id':arg}
                mongo.insertOnCrawlerQueue('Download',job_args)
                LOGGER.info('Wrote on Crawler to Download {}...OK'.format(arg))
            }elif opt == "--download-all-sources"{
                LOGGER.info('Writting on Crawler to Download All Sources...')
                mongo.insertOnCrawlerQueue('DownloadAll')
                LOGGER.info('Wrote on Crawler to Download All Sources...OK')
            }elif opt == "--check-jobs"{
                LOGGER.info('Checking jobs on Queue...')
                queues=mongo.getAllQueueJobs()
                for queue,jobs in queues.items(){
                    LOGGER.clean('Under \'{}\' queue:'.format(queue))
                    for job in jobs{
                        for k,v in job.items(){
                            tab='\t'
                            if k!='task'{
                                tab+=tab
                            }
                            LOGGER.clean('{}{}: {}'.format(tab,k.capitalize(),v))
                        }
                    }
                }
                LOGGER.info('Checked jobs on Queue...OK')
            }
        }
    } except Exception as e{
        if ITERATIVE {
            print(e)
        }else{
            raise e
        }
    }

    if zombie{
        LOGGER.info('Keeping Front end alive on Zombie Mode...')
        LOGGER.info('Tip: Use `front-end` command on container to run commands.')
        while(True){
            pass
        }
    }else{
        print()
        ITERATIVE=True
        value = input("Keeping Front end alive on Iterative Mode...\nEnter a command (e.g. -h):")
        args=value.split(' ')
        args[0]=args[0].strip()
        if len(args[0])==1{
            if not args[0].startswith('-'){
                args[0]='-{}'.format(args[0])
            }
        }else{
            if not args[0].startswith('--'){
                args[0]='--{}'.format(args[0])
            }
        }
        main(args)
    }
}


if __name__ == "__main__"{
    LOGGER.info('Starting Front end...')
    if Utils.runningOnDockerContainer(){
        mongo_addr='mongo'
    }else{
        mongo_addr='127.0.0.1'
    }
    mongo=MongoDB(mongo_addr,27017,LOGGER,user='root',password='123456')
    mongo.startQueue(id=0)
    LOGGER.info('Started Front end...OK')
    LOGGER.info('Writting on queue as {}'.format(mongo.getQueueConsumerId()))
    main(sys.argv[1:])
}