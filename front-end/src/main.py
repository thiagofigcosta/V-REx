#!/bin/python

import sys, getopt
from Utils import Utils
from Logger import Logger
from MongoDB import MongoDB

TMP_FOLDER='tmp/front/'

ITERATIVE=False

Utils.createFolderIfNotExists(TMP_FOLDER)
LOGGER=Logger(TMP_FOLDER)
Utils(TMP_FOLDER,LOGGER)

def main(argv){
    HELP_STR='main.py [-h]\n\t[--check-jobs]\n\t[--get-queue-names]\n\t[-q | --quit]\n\t[--download-source <source ID>]\n\t[--download-all-sources]\n\t[--empty-queue <queue name>]\n\t[--empty-all-queues]\n\t[--keep-alive-as-zombie]'
    args=[]
    zombie=False
    global ITERATIVE
    to_run=[]
    try{
        opts, args = getopt.getopt(argv,"hq",["keep-alive-as-zombie","download-source=","download-all-sources","check-jobs","quit","get-queue-names","empty-queue=","empty-all-queues"])
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
            }elif opt == "--download-source"{
                LOGGER.info('Writting to Crawler to Download {}...'.format(arg))
                job_args={'id':arg}
                mongo.insertOnCrawlerQueue('Download',job_args)
                LOGGER.info('Wrote to Crawler to Download {}...OK'.format(arg))
            }elif opt == "--download-all-sources"{
                LOGGER.info('Writting to Crawler to Download All Sources...')
                mongo.insertOnCrawlerQueue('DownloadAll')
                LOGGER.info('Wrote to Crawler to Download All Sources...OK')
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
            pass
        }else{
            raise e
        }
    }

    if zombie{
        LOGGER.info('Keeping Front end alive on Zombie Mode...')
        LOGGER.info('Tip: Use `front-end` cmd on container to run commands.')
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
    mongo=MongoDB('127.0.0.1',27017,LOGGER,user='root',password='123456')
    mongo.startQueue(id=0)
    LOGGER.info('Started Front end...OK')
    LOGGER.info('Writting on queue as {}'.format(mongo.getQueueConsumerId()))
    main(sys.argv[1:])
}