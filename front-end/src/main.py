#!/bin/python

import sys, getopt, bson, re
from Utils import Utils
from Logger import Logger
from MongoDB import MongoDB

TMP_FOLDER='tmp/front/'

ITERATIVE=False

Utils.createFolderIfNotExists(TMP_FOLDER)
LOGGER=Logger(TMP_FOLDER,name='front')
Utils(TMP_FOLDER,LOGGER)

def inputNumber(is_float=False,postive_number=True,greater_or_eq=None,lower_or_eq=None){
    out=0
    converted=False
    while not converted{
        try{
            if is_float{
                out=float(input())
            }else{
                out=int(input())
            }
            if (not postive_number or out >=0) and (not lower_or_eq or out <= lower_or_eq) and (not greater_or_eq or out >= greater_or_eq){ 
                converted=True
            }else{
                print('ERROR. Out of boundaries')
            }
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

def inputArrayNumber(is_float=False,postive_number=True,greater_or_eq=None,lower_or_eq=None){
    out=''
    not_converted=True
    while not_converted{
        try{
            out=input()
            out_test=out.split(',')
            for test in out_test{
                if is_float{
                    test=float(test)
                }else{
                    test=int(test)
                }
                if (not postive_number or test >=0) and (not lower_or_eq or test <= lower_or_eq) and (not greater_or_eq or test >= greater_or_eq){ 
                    not_converted=False
                }else{
                    print('ERROR. Out of boundaries')
                    not_converted=True
                    break
                }
            }
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
    HELP_STR='main.py [-h]\n\t[--check-jobs]\n\t[--create-genetic-env]\n\t[--list-genetic-envs]\n\t[--run-genetic]\n\t[--show-genetic-results]\n\t[--rm-genetic-env <env name>]\n\t[--create-smart-neural-hyperparams]\n\t[--list-smart-neural-hyperparams]\n\t[--rm-smart-neural-hyperparams <hyper name>]\n\t[--train-smart-neural]\n\t[--eval-smart-neural]\n\t[--get-queue-names]\n\t[--get-all-db-names]\n\t[-q | --quit]\n\t[--run-processor-pipeline]\n\t[--run-merge-cve]\n\t[--run-flattern-and-simplify-all]\n\t[--run-flattern-and-simplify [cve|oval|capec|cwe]]\n\t[--run-filter-exploits]\n\t[--run-transform-all]\n\t[--run-transform [cve|oval|capec|cwe|exploits]]\n\t[--run-enrich]\n\t[--run-analyze]\n\t[--run-filter-and-normalize]\n\t[--download-source <source ID>]\n\t[--download-all-sources]\n\t[--empty-queue <queue name>]\n\t[--empty-all-queues]\n\t[--dump-db <db name>#<folder path to export> | --dump-db <db name> {saves on default tmp folder} \n\t\te.g. --dump-db queue#/home/thiago/Desktop]\n\t[--restore-db <file path to import>#<db name> | --restore-db <file path to import> {saves db under file name} \n\t\te.g. --restore-db /home/thiago/Desktop/queue.zip#restored_queue]\n\t[--keep-alive-as-zombie]'
    args=[]
    zombie=False
    global ITERATIVE
    to_run=[]
    try{ 
        opts, args = getopt.getopt(argv,"hq",["keep-alive-as-zombie","download-source=","download-all-sources","check-jobs","quit","get-queue-names","empty-queue=","empty-all-queues","get-all-db-names","dump-db=","restore-db=","run-processor-pipeline","run-flattern-and-simplify-all","run-flattern-and-simplify=","run-filter-exploits","run-transform-all","run-transform=","run-enrich","run-analyze","run-filter-and-normalize","run-merge-cve","create-genetic-env","list-genetic-envs","rm-genetic-env=","run-genetic","show-genetic-results","create-smart-neural-hyperparams","list-smart-neural-hyperparams","rm-smart-neural-hyperparams=","train-smart-neural","eval-smart-neural"])
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
            }elif opt == "--list-smart-neural-hyperparams"{
                LOGGER.info('Getting hyperparameters...')
                for hyp in mongo.findAllOnDB(mongo.getDB('neural_db'),'snn_hyperparameters',wait_unlock=False){
                    for k,v in hyp.items(){
                        if k!='_id'{
                            LOGGER.clean('{}: {}'.format(k,str(v)))
                        }
                    }
                    LOGGER.clean('\n')
                }
                LOGGER.info('Gotten hyperparameters...OK')
            }elif opt == "--create-smart-neural-hyperparams"{
                print('Now type the hyperparameters for the Smart Neural Network...')
                print('Enter the hyperparameters config name (unique): ', end = '')
                hyper_name=input().strip()
                submitted_at=Utils.getTodayDate('%d/%m/%Y %H:%M:%S')
                print('Enter the batch size: ', end = '')
                batch_size=inputNumber()
                print('Enter the alpha: ', end = '')
                alpha=inputNumber(is_float=True,lower_or_eq=1)
                print('Enter 1 to shuffle train data or 0 otherwise: ', end = '')
                shuffle=inputNumber(lower_or_eq=1)==1
                print('Enter 1 to use adam optimizer or 0 otherwise: ', end = '')
                adam=inputNumber(lower_or_eq=1)==1
                print('Enter the rehash (6400): ', end = '')
                rehash=inputNumber()
                print('Enter the rebuild (128000): ', end = '')
                rebuild=inputNumber()
                print('Enter the label type (0-2):')
                print('\t0 - INT_CLASS')
                print('\t1 - NEURON_BY_NEURON')
                print('\t2 - NEURON_BY_NEURON_LOG_LOSS')
                print('value: ', end='')
                label_type=inputNumber(lower_or_eq=2)
                print('Enter amount of layers: ', end = '')
                layers=inputNumber(greater_or_eq=1)
                layer_sizes=[]
                for i in range(layers){
                    if i==0{
                        print('Enter the layer size for layer 0 (output size): ', end = '')
                    }else{
                        print('Enter the layer size for layer {}: '.format(i), end = '')
                    }
                    layer_sizes.append(inputNumber())
                }
                range_pow=[]
                for i in range(layers){
                    print('Enter the range pow for layer {}: '.format(i), end = '')
                    range_pow.append(inputNumber())
                }
                K=[]
                for i in range(layers){
                    print('Enter the K for layer {}: '.format(i), end = '')
                    K.append(inputNumber())
                }
                L=[]
                for i in range(layers){
                    print('Enter the L for layer {}: '.format(i), end = '')
                    L.append(inputNumber())
                }
                if layers > 1 {
                    print('Node types (0-2):')
                    print('\t0 - ReLU')
                    print('\t1 - Softmax')
                    print('\t2 - Sigmoid')
                    print('value: ', end='')
                }
                node_types=[]
                for i in range(layers-1){
                    print('Enter the node type for layer {}: '.format(i), end = '')
                    node_types.append(inputNumber(lower_or_eq=2))
                }
                node_types.append(1) # softmax
                sparcity=[1] # border
                for i in range(layers-2){
                    print('Enter the sparcity for layer {}: '.format(i), end = '')
                    sparcity.append(inputNumber(is_float=True,lower_or_eq=1))
                }
                if layers > 1 {
                    sparcity.append(1); # border
                }
                hyperparams={'name':hyper_name,'submitted_at':submitted_at,'batch_size':batch_size,'alpha':alpha,'shuffle':shuffle,'adam':adam,'rehash':rehash,'rebuild':rebuild,'label_type':label_type,'layers':layers,'layer_sizes':layer_sizes,'range_pow':range_pow,'K':K,'L':L,'node_types':node_types,'sparcity':sparcity}
                LOGGER.info('Writting hyperparameters on neural_db...')
                mongo.insertOneOnDB(mongo.getDB('neural_db'),hyperparams,'snn_hyperparameters',index='name',ignore_lock=True)
                LOGGER.info('Wrote hyperparameters on neural_db...OK')
            }elif opt == "--list-genetic-envs"{
                LOGGER.info('Getting genetic environments...')
                for env in mongo.findAllOnDB(mongo.getDB('genetic_db'),'environments',wait_unlock=False){
                    LOGGER.clean('Name: {}'.format(env['name']))
                    LOGGER.clean('Submitted At: {}'.format(env['submitted_at']))
                    LOGGER.clean('Space Search:')
                    for k,v in env['space_search'].items() {
                        LOGGER.clean('\t{}: {}'.format(k,str(v)))
                    }
                    LOGGER.clean('\n')
                }
                LOGGER.info('Gotten genetic environments...OK')
            }elif opt == "--eval-smart-neural"{
                print('Enter a existing independent network name to be used: ', end = '')
                independent_net_name=input().strip()
                independent_net=mongo.findOneOnDBFromIndex(mongo.getDB('neural_db'),'independent_net','name',independent_net_name,wait_unlock=False)
                if not independent_net{
                    LOGGER.error('Not found a independent network for the given name!')
                }else{
                    result_info={'total_test_cases':0,'correct_predictions(not ground truth)':0,'predicted_labels':None}
                    LOGGER.info('Writting eval result holder...')
                    result_id=mongo.quickInsertOneIgnoringLockAndRetrieveId(mongo.getDB('neural_db'),result_info,'eval_results')
                    LOGGER.info('Wrote eval result holder...OK')
                    if (result_id==None){
                        LOGGER.error('Failed to insert eval result holder!')
                    }else{
                        print('Eval single CVE or multiple (0 - single | 1 - multiple): ')
                        if inputNumber(lower_or_eq=1)==1{
                            print('Enter the years to be used as eval data splitted by comma (,) (1999-2020):')
                            eval_data=inputArrayNumber(greater_or_eq=1999,lower_or_eq=2020)
                            print('Enter a limit of CVEs for each year (0 = unlimitted): ')
                            eval_data+=':{}'.format(inputNumber())
                            job_args={'eval_data':eval_data,'result_id':result_id,'independent_net_id':str(independent_net['_id'])}
                        }else{
                            print('Enter a CVE id following the format CVE-####-#*: ')
                            not_filled=True
                            eval_cve=''
                            while not_filled {
                                eval_cve=input().strip()
                                if re.match(r'^CVE-[0-9]{4}-[0-9]*$',eval_cve){
                                    not_filled=False
                                }else{
                                    print('ERROR - Wrong CVE format')
                                }
                            }
                            job_args={'eval_data':eval_cve,'result_id':result_id,'independent_net_id':str(independent_net['_id'])}
                        }
                        LOGGER.info('Writting on Core queue to eval network...')
                        mongo.insertOnCoreQueue('Eval SNN',job_args)
                        LOGGER.info('Wrote on Core queue to eval network...OK')
                    }
                }
            }elif opt == "--train-smart-neural"{
                print('Now enter the data to train the neural network...')
                print('Enter a existing hyperparameters name to be used: ', end = '')
                hyper_name=input().strip()
                hyper=mongo.findOneOnDBFromIndex(mongo.getDB('neural_db'),'snn_hyperparameters','name',hyper_name,wait_unlock=False)
                if not hyper{
                    LOGGER.error('Not found a hyperparameter for the given name!')
                }else{
                    print('Enter a name for the train (unique): ', end = '')
                    train_name=input().strip()
                    submitted_at=Utils.getTodayDate('%d/%m/%Y %H:%M:%S')
                    started_by=None
                    started_at=None
                    finished_at=None
                    weights=None
                    print('Enter the epochs: ',end='')
                    epochs=inputNumber()
                    print('Enter the cross validation method (0-4):')
                    print('\t0 - NONE')
                    print('\t1 - ROLLING_FORECASTING_ORIGIN')
                    print('\t2 - KFOLDS')
                    print('\t3 - TWENTY_PERCENT')
                    print('value: ', end='')
                    cross_validation=inputNumber(lower_or_eq=3)
                    print('Enter the metric to be used during training/val (0-4):')
                    print('\t0 - RAW_LOSS (cheaper)')
                    print('\t1 - F1')
                    print('\t2 - RECALL')
                    print('\t3 - ACCURACY')
                    print('\t4 - PRECISION')
                    print('value: ',end='')
                    train_metric=inputNumber(lower_or_eq=4)
                    print('Enter the years to be used as train data splitted by comma (,) (1999-2020):')
                    train_data=inputArrayNumber(greater_or_eq=1999,lower_or_eq=2020)
                    print('Enter a limit of CVEs for each year (0 = unlimitted): ')
                    train_data+=':{}'.format(inputNumber())
                    print('Run test data also (0 - no | 1 - yes): ')
                    if inputNumber(lower_or_eq=1)==1{
                        print('Enter the metric to be used during test (0-4):')
                        print('\t0 - RAW_LOSS (cheaper)')
                        print('\t1 - F1')
                        print('\t2 - RECALL')
                        print('\t3 - ACCURACY')
                        print('\t4 - PRECISION')
                        print('value: ',end='')
                        test_metric=inputNumber(lower_or_eq=4)

                        print('Enter the years to be used as test data splitted by comma (,) (1999-2020):')
                        test_data=inputArrayNumber(greater_or_eq=1999,lower_or_eq=2020)
                        print('Enter a limit of CVEs for each year (0 = unlimitted): ')
                        test_data+=':{}'.format(inputNumber())
                    }else{
                        test_metric=0
                        test_data=''
                    }
                    train_metadata={'name':train_name,'hyperparameters_name':hyper_name,'submitted_at':submitted_at,'started_by':started_by,'started_at':started_at,'finished_at':finished_at,'epochs':epochs,'cross_validation':cross_validation,'train_metric':train_metric,'train_data':train_data,'test_metric':test_metric,'test_data':test_data,'weights':weights}
                    LOGGER.info('Writting simulation config on genetic_db...')
                    independent_net_id=mongo.quickInsertOneIgnoringLockAndRetrieveId(mongo.getDB('neural_db'),train_metadata,'independent_net',index='name')
                    if (independent_net_id==None){
                        LOGGER.error('Failed to insert simulation!')
                    }else{
                        LOGGER.info('Wrote simulation config on genetic_db...OK')

                        LOGGER.info('Writting on Core queue to train network...')
                        job_args={'independent_net_id':independent_net_id}
                        mongo.insertOnCoreQueue('Train SNN',job_args)
                        LOGGER.info('Wrote on Core queue to train network...OK')
                    }
                }
            }elif opt == "--run-genetic"{
                print('Now enter the data to run the genetic experiment...')
                print('Enter a existing genetic environment name to be used: ', end = '')
                env_name=input().strip()
                env=mongo.findOneOnDBFromIndex(mongo.getDB('genetic_db'),'environments','name',env_name,wait_unlock=False)
                if not env{
                    LOGGER.error('Not found an environment for the given name!')
                }else{
                    print('Enter a name for the simulation (not unique, just for reference): ', end = '')
                    simulation_name=input().strip()
                    submitted_at=Utils.getTodayDate('%d/%m/%Y %H:%M:%S')
                    started_by=None
                    started_at=None
                    finished_at=None
                    hall_of_fame_id=None
                    population_id=None
                    print('Enter the population start size: ')
                    pop_start_size=inputNumber()
                    print('Enter the amount of generations: ')
                    max_gens=inputNumber()
                    print('Enter the algorithm to use (0 - Enchanced | 1 - Standard): ')
                    algorithm=inputNumber(lower_or_eq=1)
                    print('Enter the genome max age: ')
                    max_age=inputNumber()
                    print('Enter the max amount of children at once: ')
                    max_children=inputNumber()
                    print('Enter the mutation rate: ')
                    mutation_rate=inputNumber(is_float=True,lower_or_eq=1)
                    print('Enter the recycle rate: ')
                    recycle_rate=inputNumber(is_float=True,lower_or_eq=1)
                    print('Enter the sex rate: ')
                    sex_rate=inputNumber(is_float=True,lower_or_eq=1)
                    print('Enter the max amount of notable individuals at the Hall Of Fame: ')
                    max_notables=inputNumber()
                    print('Enter the cross validation method (0-3):')
                    print('\t0 - NONE')
                    print('\t1 - ROLLING_FORECASTING_ORIGIN')
                    print('\t2 - KFOLDS')
                    print('\t3 - TWENTY_PERCENT')
                    print('value: ')
                    cross_validation=inputNumber(lower_or_eq=3)
                    print('Enter the metric to be used (0-4):')
                    print('\t0 - RAW_LOSS (cheaper)')
                    print('\t1 - F1')
                    print('\t2 - RECALL')
                    print('\t3 - ACCURACY')
                    print('\t4 - PRECISION')
                    print('value: ')
                    metric=inputNumber(lower_or_eq=4)
                    print('Enter the years to be used as train data splitted by comma (,) (1999-2020):')
                    train_data=inputArrayNumber(greater_or_eq=1999,lower_or_eq=2020)
                    print('Enter a limit of CVEs for each year (0 = unlimitted): ')
                    train_data+=':{}'.format(inputNumber())
                    best={'output':None,'at_gen':None}
                    results=[]
                    simulation_data={'name':simulation_name,'env_name':env_name,'submitted_at':submitted_at,'started_by':started_by,'started_at':started_at,'finished_at':finished_at,'hall_of_fame_id':hall_of_fame_id,'population_id':population_id,'pop_start_size':pop_start_size,'max_gens':max_gens,'algorithm':algorithm,'max_age':max_age,'max_children':max_children,'mutation_rate':mutation_rate,'recycle_rate':recycle_rate,'sex_rate':sex_rate,'max_notables':max_notables,'cross_validation':cross_validation,'metric':metric,'train_data':train_data,'best':best,'results':results}
                    LOGGER.info('Writting simulation config on genetic_db...')
                    simulation_id=mongo.quickInsertOneIgnoringLockAndRetrieveId(mongo.getDB('genetic_db'),simulation_data,'simulations')
                    if (simulation_id==None){
                        LOGGER.error('Failed to insert simulation!')
                    }else{
                        LOGGER.info('Wrote simulation config on genetic_db...OK')
                        halloffame_data={'simulation_id':simulation_id,'env_name':env_name,'updated_at':None,'neural_genomes':[]}
                        halloffame_id=mongo.quickInsertOneIgnoringLockAndRetrieveId(mongo.getDB('neural_db'),halloffame_data,'hall_of_fame')
                        generation_data={'simulation_id':simulation_id,'env_name':env_name,'updated_at':None,'neural_genomes':[]}
                        population_id=mongo.quickInsertOneIgnoringLockAndRetrieveId(mongo.getDB('neural_db'),generation_data,'populations')
                        if halloffame_id == None or population_id == None{
                            LOGGER.error('Failed to insert hall of fame or/and generation!')
                        }else{
                            query={'_id':bson.ObjectId(simulation_id)}
                            update={'$set':{'hall_of_fame_id':halloffame_id,'population_id':population_id}}
                            mongo.getDB('genetic_db')['simulations'].find_one_and_update(query,update)

                            LOGGER.info('Writting on Core queue to run genetic simulation...')
                            job_args={'simulation_id':simulation_id}
                            mongo.insertOnCoreQueue('Genetic',job_args)
                            LOGGER.info('Wrote on Core queue to run genetic simulation...OK')
                        }
                    }
                }
            }elif opt == "--show-genetic-results"{
                LOGGER.info('Getting genetic results...')
                for sim in mongo.findAllOnDB(mongo.getDB('genetic_db'),'simulations',wait_unlock=False){
                    for k,v in sim.items(){
                        if k=="hall_of_fame_id"{
                            hall=mongo.findOneOnDBFromIndex(mongo.getDB('neural_db'),'hall_of_fame','_id',bson.ObjectId(v),wait_unlock=False)
                            LOGGER.clean('Hall Of Fame:')
                            if (len( hall['neural_genomes'])==0){
                                LOGGER.clean('\t[]')
                            }else{
                                for el in hall['neural_genomes']{
                                    LOGGER.clean('\t{}'.format(str(el))) 
                                }
                            }
                        }elif k=="population_id"{
                            pop=mongo.findOneOnDBFromIndex(mongo.getDB('neural_db'),'populations','_id',bson.ObjectId(v),wait_unlock=False)
                            LOGGER.clean('Population:')
                            if (len(pop['neural_genomes'])==0){
                                LOGGER.clean('\t[]')
                            }else{
                                for el in pop['neural_genomes']{
                                    LOGGER.clean('\t{}:'.format(str(el))) 
                                }
                            }
                        }else{
                             if type(v) is list{
                                if (len(v)==0){
                                    LOGGER.clean('{}: []'.format(k))
                                }else{
                                    LOGGER.clean('{}:'.format(k))
                                    for el in v{
                                        LOGGER.clean('\t{}:'.format(str(el))) 
                                    }
                                }
                            }else{
                                LOGGER.clean('{}: {}'.format(k,str(v)))
                            }
                        }
                    }
                    LOGGER.clean('\n')
                }
                LOGGER.info('Gotten genetic results...OK')
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
                alpha_min=inputNumber(is_float=True,lower_or_eq=1)
                print("Max: ")
                alpha_max=inputNumber(is_float=True,lower_or_eq=1)
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
                sparcity_min=inputNumber(is_float=True,lower_or_eq=1)
                print("Max: ")
                sparcity_max=inputNumber(is_float=True,lower_or_eq=1)
                print('Enter the activation functions (0-2):')
                print('\t0 - ReLU')
                print('\t1 - Softmax')
                print('\t2 - Sigmoid')
                print('min: ')
                activation_min=inputNumber(lower_or_eq=2)
                print("Max: ")
                activation_max=inputNumber(lower_or_eq=2)
                space_search={'name':gen_name,'submitted_at':submitted_at,'space_search':{'amount_of_layers':{'min':amount_of_layers_min,'max':amount_of_layers_max},'epochs':{'min':epochs_min,'max':epochs_max},'batch_size':{'min':batch_size_min,'max':batch_size_max},'layer_sizes':{'min':layer_size_min,'max':layer_size_max},'range_pow':{'min':range_pow_min,'max':range_pow_max},'K':{'min':k_min,'max':k_max},'L':{'min':l_min,'max':l_max},'activation_functions':{'min':activation_min,'max':activation_max},'sparcity':{'min':sparcity_min,'max':sparcity_max},'alpha':{'min':alpha_min,'max':alpha_max}}}
                LOGGER.info('Writting environment on genetic_db...')
                mongo.insertOneOnDB(mongo.getDB('genetic_db'),space_search,'environments',index='name',ignore_lock=True)
                LOGGER.info('Wrote environment on genetic_db...OK')
            }elif opt == "--rm-genetic-env"{
                arg=arg.strip()
                LOGGER.info('Removing {} from genetic environments...'.format(arg))
                query={'name':arg}
                mongo.rmOneFromDB(mongo.getDB('genetic_db'),'environments',query=query)
                LOGGER.info('Removed {} from genetic environments...OK'.format(arg))
            }elif opt == "--rm-smart-neural-hyperparams"{
                arg=arg.strip()
                LOGGER.info('Removing {} from hyperparameters...'.format(arg))
                query={'name':arg}
                mongo.rmOneFromDB(mongo.getDB('neural_db'),'snn_hyperparameters',query=query)
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