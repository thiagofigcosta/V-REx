#include <iostream>
#include <signal.h>
#include <boost/stacktrace.hpp>
#include <exception>
#include <stdexcept>
#include <sys/resource.h>
#include <omp.h>

#include "Utils.hpp"
#include "NeuralGenome.hpp"
#include "test.hpp"
#include "Slide.hpp"

using namespace std;

MongoDB* mongo=nullptr;
const bool trap_signals=false;
const bool connect_mongo=true;

void exceptionHandler(int signum) {
    ::signal(signum, SIG_DFL);
    // boost::stacktrace::safe_dump_to("./backtrace.dump");
    cerr << endl << "Stack trace:" << endl << boost::stacktrace::stacktrace() << endl;
    try {
        exception_ptr eptr=current_exception();
        if (eptr) {
            rethrow_exception(eptr);
        }
    } catch(const exception& e) {
        cerr << "Caught exception:\n\t\"" << e.what() << "\"\n\n";
    }
    if(!NeuralGenome::last_print_str.empty()){
        cout<<"\n\nOn "+NeuralGenome::last_print_str<<endl;
    }
    ::raise(SIGABRT);
}

void setup(){
    if (trap_signals){
        cout<<"Setting up signal trap...\n";
        ::signal(SIGSEGV, &exceptionHandler);
        ::signal(SIGABRT, &exceptionHandler);
        cout<<"Set up signal trap...OK\n";
    }
    // cout<<"Setting up stack size...\n";
    // const rlim_t kStackSize = 1 * 1024 * 1024 * 1024;   // 1 * 1024 * 1024 * 1024 = 1 GB
    // struct rlimit rl;
    // int result;
    // result = getrlimit(RLIMIT_STACK, &rl);
    // if (result == 0){
    //     if (rl.rlim_cur < kStackSize){
    //         rl.rlim_cur = kStackSize;
    //         result = setrlimit(RLIMIT_STACK, &rl);
    //         if (result != 0){
    //             fprintf(stderr, "setrlimit returned result = %d\n", result);
    //         }
    //     }
    // }
    // cout<<"Set up stack size...OK\n";
    if(connect_mongo){
        cout<<"Connecting on Mongo...\n";
        string mongo_host;
        if (Utils::runningOnDockerContainer()){
            mongo_host="mongo";
        }else{
            mongo_host="127.0.0.1";
        }
        mongo = new MongoDB(mongo_host,"root","123456");
        cout<<"Connected on Mongo...OK\n";
    }
    if (Slide::MAX_THREADS > 0){
        cout<<"Limiting amount of threads...\n";
        omp_set_dynamic(0);
        omp_set_num_threads(Slide::MAX_THREADS);
        cout<<"Limited amount of threads...OK\n";
    }
}

void tearDown(){
    if(mongo){
        delete mongo;
        mongo=nullptr;
    }
}

void runGeneticSimulation(string simulation_id){
    cout<<"Running genetic simulation "+simulation_id+"...\n";
    pair<vector<string>,vector<float>> simu_data=mongo->fetchGeneticSimulationData(simulation_id);
    string environment_name=simu_data.first[0];
    vector<string> str_cve_years=Utils::splitString(simu_data.first[1],",");
    string hall_of_fame_id=simu_data.first[2];
    string population_id=simu_data.first[3];
    int population_start_size=(int)simu_data.second[0];
    int max_gens=(int)simu_data.second[1];
    int max_age=(int)simu_data.second[3];
    int max_children=(int)simu_data.second[4];
    float mutation_rate=simu_data.second[5];
    float recycle_rate=simu_data.second[6];
    float sex_rate=simu_data.second[7];
    int max_notables=(int)simu_data.second[8];
    SlideCrossValidation cross_validation;
    switch((int)simu_data.second[9]){
        case 0:
            cross_validation=SlideCrossValidation::NONE;
            break;
        case 1:
            cross_validation=SlideCrossValidation::ROLLING_FORECASTING_ORIGIN;
            break;
        case 2:
            cross_validation=SlideCrossValidation::KFOLDS;
            break;
        case 3:
            cross_validation=SlideCrossValidation::TWENTY_PERCENT;
            break;
    }
    SlideMetric metric_mode;
    switch((int)simu_data.second[10]){
        case 0:
            metric_mode=SlideMetric::RAW_LOSS;
            break;
        case 1:
            metric_mode=SlideMetric::F1;
            break;
        case 2:
            metric_mode=SlideMetric::RECALL;
            break;
        case 3:
            metric_mode=SlideMetric::ACCURACY;
            break;
        case 4:
            metric_mode=SlideMetric::PRECISION;
            break;
    }
    int train_data_limit=(int)simu_data.second[11];
    simu_data.first.clear();
    simu_data.second.clear();

    SPACE_SEARCH search_space = mongo->fetchEnvironmentData(environment_name);

    vector<int> cve_years;
    for(string y:str_cve_years){
        cve_years.push_back(stoi(y));
    }
    str_cve_years.clear();

    vector<pair<vector<int>, vector<float>>> train_data = mongo->loadCvesFromYears(cve_years, train_data_limit).second;

    const bool shuffle_train_data=false;
    const int rehash=6400;
    const int rebuild=128000;
    const int border_sparsity=1; // first and last layers
    const SlideLabelEncoding label_encoding=SlideLabelEncoding::INT_CLASS;
    const int input_size=train_data[0].second.size();
    const int output_size=train_data[0].first.size();
    const bool adam_optimizer=true;
    const bool use_neural_genome=true;
    const bool search_maximum=(metric_mode!=SlideMetric::RAW_LOSS);
    NeuralGenome::CACHE_WEIGHTS=true;

    auto train_callback = [&](Genome *self) -> float {
        auto self_neural=dynamic_cast<NeuralGenome*>(self);

        tuple<Slide*,int,function<void()>> net=self_neural->buildSlide(self->getDna(),input_size,output_size,label_encoding,rehash,rebuild,border_sparsity,metric_mode,shuffle_train_data,cross_validation,adam_optimizer);
        if (self_neural->hasWeights()){
            get<0>(net)->setWeights(self_neural->getWeights());
            self_neural->clearWeights();
        }

        vector<pair<float,float>> metric=get<0>(net)->train(train_data,get<1>(net));
        self_neural->setWeights(get<0>(net)->getWeights());
        delete get<0>(net); // free memory
        get<2>(net)(); // free memory
        float output=0;
        for(pair<float,float> l:metric){
            if (cross_validation!=SlideCrossValidation::NONE){
                output+=l.second; // use validation
            }else{
                output+=l.first; // use validation
            }
        }
        output/=metric.size();
        return output;
    };

    auto after_gen_callback = [&](int pop_size,int g,float best_out,long timestamp_ms,vector<Genome*> population,HallOfFame *hall_of_fame) -> void {
        if (hall_of_fame){
            mongo->updateBestOnGeneticSimulation(simulation_id,hall_of_fame->getBest(),Utils::getStrNow());
        }
        mongo->appendResultOnGeneticSimulation(simulation_id,pop_size,g,best_out,timestamp_ms);
        mongo->clearPopulationNeuralGenomeVector(population_id,Utils::getStrNow());
        for (Genome* g:population){
            mongo->addToPopulationNeuralGenomeVector(population_id,dynamic_cast<NeuralGenome*>(g),Utils::getStrNow());
        }
    };
    
    mongo->claimGeneticSimulation(simulation_id,Utils::getStrNow(),Utils::getHostname());
    HallOfFame elite=HallOfFame(max_notables, search_maximum);
    EnchancedGenetic en_ga = EnchancedGenetic(max_children,max_age,mutation_rate,sex_rate,recycle_rate);
    PopulationManager enchanced_population=PopulationManager(en_ga,search_space,train_callback,population_start_size,search_maximum,use_neural_genome,true,after_gen_callback);
    enchanced_population.setHallOfFame(elite);
    cout<<"Starting natural selection"<<endl;
    enchanced_population.naturalSelection(max_gens);
    cout<<"Finished natural selection"<<endl;
    cout<<"Best loss ("<<elite.getBest().second<<"): "<<elite.getBest().first<<endl;
    for (Genome* g:elite.getNotables()){
        mongo->addToHallOfFameNeuralGenomeVector(hall_of_fame_id,dynamic_cast<NeuralGenome*>(g),Utils::getStrNow());
    }
    mongo->finishGeneticSimulation(simulation_id,Utils::getStrNow());

    cout<<"Runned genetic simulation "+simulation_id+"...OK\n";
}

void trainNeuralNetwork(string independent_net_id){
    cout<<"Training neural network "+independent_net_id+"...\n";
    pair<vector<string>,vector<int>> train_mdata=mongo->fetchNeuralNetworkTrainMetadata(independent_net_id);
    string hyper_name=train_mdata.first[0];
    vector<string> str_cve_years_train=Utils::splitString(train_mdata.first[1],",");
    vector<string> str_cve_years_test;
    if (!train_mdata.first[2].empty()){
        str_cve_years_test=Utils::splitString(train_mdata.first[2],",");
    }
    int epochs=train_mdata.second[0];
    SlideCrossValidation cross_validation=SlideCrossValidation::NONE;
    switch(train_mdata.second[1]){
        case 0:
            cross_validation=SlideCrossValidation::NONE;
            break;
        case 1:
            cross_validation=SlideCrossValidation::ROLLING_FORECASTING_ORIGIN;
            break;
        case 2:
            cross_validation=SlideCrossValidation::KFOLDS;
            break;
        case 3:
            cross_validation=SlideCrossValidation::TWENTY_PERCENT;
            break;
    }
    SlideMetric train_metric=SlideMetric::RAW_LOSS;
    switch(train_mdata.second[2]){
        case 0:
            train_metric=SlideMetric::RAW_LOSS;
            break;
        case 1:
            train_metric=SlideMetric::F1;
            break;
        case 2:
            train_metric=SlideMetric::RECALL;
            break;
        case 3:
            train_metric=SlideMetric::ACCURACY;
            break;
        case 4:
            train_metric=SlideMetric::PRECISION;
            break;
    }
    // SlideMetric test_metric=SlideMetric::RAW_LOSS; // useless
    int train_limit=train_mdata.second[4];
    int test_limit=train_mdata.second[5];
    vector<int> cve_years_train;
    for(string y:str_cve_years_train){
        cve_years_train.push_back(stoi(y));
    }
    str_cve_years_train.clear();

    Hyperparameters* hyper=mongo->fetchHyperparametersData(hyper_name);

    vector<pair<vector<int>, vector<float>>> train_data = mongo->loadCvesFromYears(cve_years_train, train_limit).second;
   
    mongo->claimNeuralNetTrain(independent_net_id,Utils::getStrNow(),Utils::getHostname());
    const bool print_deltas=true;
    Slide* slide=new Slide(hyper->layers,hyper->layer_sizes,hyper->node_types,train_data[0].second.size(),hyper->alpha,hyper->batch_size,hyper->adam,hyper->label_type,
    hyper->range_pow,hyper->K,hyper->L,hyper->sparcity,hyper->rehash,hyper->rebuild,train_metric,train_metric,hyper->shuffle,cross_validation,SlideMode::SAMPLING,SlideHashingFunction::DENSIFIED_WTA,print_deltas);
    vector<pair<float,float>> train_metrics=slide->train(train_data,epochs);
    vector<vector<pair<int,float>>> train_predicted=slide->evalData(train_data).second;
    snn_stats train_stats=Utils::statisticalAnalysis(train_data,train_predicted);
    mongo->appendTMetricsOnNeuralNet(independent_net_id,train_metrics);
    mongo->appendStatsOnNeuralNet(independent_net_id,"train_stats",train_stats);
    mongo->appendWeightsOnNeuralNet(independent_net_id,slide->getWeights());
    if (str_cve_years_test.size()>0){
        for (pair<vector<int>, vector<float>> v:train_data){
            v.first.clear();
            v.second.clear();
        }
        train_data.clear();
        vector<int> cve_years_test;
        for(string y:str_cve_years_test){
            cve_years_test.push_back(stoi(y));
        }
        str_cve_years_test.clear();
        vector<pair<vector<int>, vector<float>>> test_data = mongo->loadCvesFromYears(cve_years_test, test_limit).second;
        vector<vector<pair<int,float>>> test_predicted=slide->evalData(test_data).second;
        snn_stats test_stats=Utils::statisticalAnalysis(test_data,train_predicted);
        mongo->appendStatsOnNeuralNet(independent_net_id,"test_stats",test_stats);
    }
    mongo->finishNeuralNetTrain(independent_net_id,Utils::getStrNow());
    cout<<"Trained neural network "+independent_net_id+"...OK\n";
    delete slide;
    delete hyper;
}

void evalNeuralNetwork(string independent_net_id, string eval_data){
    // TODO check if data is CVE or several CVES
    // TODO everything else
}

int main() {
    setup();
    // test();
    // runGeneticSimulation("601b063dbc85e6419b8462ca");
    trainNeuralNetwork("601b6576d6b384e479047d56");
    tearDown();
    return 0;
}