#!/bin/python

import sys, getopt
from Utils import Utils
from Logger import Logger
from MongoDB import MongoDB

TMP_FOLDER='tmp/front/'

Utils.createFolderIfNotExists(TMP_FOLDER)
LOGGER=Logger(TMP_FOLDER)
Utils(TMP_FOLDER,LOGGER)

def main(argv){
    HELP_STR='main.py [-h]\n\t[--check-jobs]\n\t[--get-queue-names]\n\t[-q | --quit]\n\t[--download-source <source ID>]\n\t[--download-all-sources]\n\t[--empty-queue <queue name>]\n\t[--keep-alive-as-zombie]'
    args=[]
    zombie=False
    to_run=[]
    try{
        opts, args = getopt.getopt(argv,"hq",["keep-alive-as-zombie","download-source=","download-all-sources","check-jobs","quit","get-queue-names","empty-queue="])
    }except getopt.GetoptError{
        print (HELP_STR)
        sys.exit(2)
    }
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
                LOGGER.clean('Under queue {}:'.format(queue.capitalize()))
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

    if zombie{
        LOGGER.info('Keeping Front end alive on Zombie Mode...')
        LOGGER.info('Tip: Use `front-end` cmd on container to run commands.')
        while(True){
            pass
        }
    }else{
        print()
        value = input("Keeping Front end alive on Iterative Mode...\nEnter a command (e.g. -h):")
        main(value.split(' '))
    }
}


if __name__ == "__main__"{
    LOGGER.info('Starting Front end...')
    mongo=MongoDB('127.0.0.1',27017,LOGGER,user='root',password='123456')
    mongo.startQueue(id=0) # TODO get ID from enviroment var
    LOGGER.info('Started Front end...OK')
    LOGGER.info('Writting on queue as {}'.format(mongo.getQueueConsumerId()))
    main(sys.argv[1:])
}