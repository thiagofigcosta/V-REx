#include "MongoDB.hpp"

// mongocxx::instance MongoDB::inst{}; // This should be done only once.

MongoDB::MongoDB(string host, string user, string password, int port) {
    // to fix mongo driver run the commands below if needed
    // sudo ln -s /usr/local/lib/libmogocxx.so.3.4.0 libmongocxx.so._noabi
    // sudo ln -s /usr/local/lib/libmongocxx.so._noabi /usr/local/lib/libmongocxx.so
    mongocxx::instance instance{}; // This should be done only once.
    string conn_str="mongodb://"+user+":"+password+"@"+host+":"+to_string(port)+"/?authSource=admin";
    client = MongoDB::getClient(conn_str);
}

MongoDB::MongoDB(const MongoDB& orig) {
}

MongoDB::~MongoDB() {
    // client.~client(); // causes to crash
}

mongocxx::client MongoDB::getClient(const string &conn_str){
    mongocxx::client client{mongocxx::uri{conn_str}};
    return client;
}

mongocxx::database MongoDB::getDB(const string &db_name){
    return client[db_name];
}

mongocxx::collection MongoDB::getCollection(const mongocxx::database &db, const string &col_name){
    return db[col_name];
}

pair<string,pair<vector<int>, vector<float>>> MongoDB::bsonToDatasetEntry(const bsoncxx::v_noabi::document::view bson){
    int features_size;
    if (Utils::runningOnDockerContainer()){
        features_size=878; // I used diffferent data on production environment
    }else{
        features_size=863;
    }
    const int labels_size=8;
    
    string cve_id="";
    bsoncxx::document::element cve_el = bson["cve"];
    if (cve_el.type() == bsoncxx::type::k_utf8){
        cve_id=cve_el.get_utf8().value.data();
    }else{
        throw runtime_error("Error invalid type for CVE: "+bsoncxx::to_string(cve_el.type())+"\n");
    }
    vector<float> features;
    bsoncxx::document::element features_el = bson["features"];
    if (features_el.type() == bsoncxx::type::k_document){
        bsoncxx::document::view features_vw=features_el.get_document().view();
        map<string,float> features_map;
        for (auto el:features_vw){
            string key=el.key().data();
            if (el.type() == bsoncxx::type::k_int32){
                features_map[key]=(float)el.get_int32();
            }else if (el.type() == bsoncxx::type::k_double){
                features_map[key]=(float)el.get_double();
            }else{
                throw runtime_error("Error invalid type: "+bsoncxx::to_string(el.type())+"\n");
            }
        }
        if((int)features_map.size()!=features_size){
            throw runtime_error("Error features sizes for "+cve_id+" should be: "+to_string(features_size)+" but is: "+to_string(features_map.size())+"\n");
        }
        for(map<string,float>::const_iterator it=features_map.begin();it!=features_map.end();it++){
            features.push_back(it->second);
        }
    }else{
        throw runtime_error("Error unkown features type for "+cve_id+": "+bsoncxx::to_string(features_el.type())+"\n");
    }
    vector<int> labels;
    bsoncxx::document::element labels_el = bson["labels"];
    if (labels_el.type() == bsoncxx::type::k_document){
        bsoncxx::document::view labels_vw=labels_el.get_document().view();
        map<string,float> labels_map;
        for (auto el:labels_vw){
            string key=el.key().data();
            if (el.type() == bsoncxx::type::k_int32){
                labels_map[key]=(float)el.get_int32();
            }else if (el.type() == bsoncxx::type::k_double){
                labels_map[key]=(float)el.get_double();
            }else{
                throw runtime_error("Error invalid type: "+bsoncxx::to_string(el.type())+"\n");
            }
        }
        if((int)labels_map.size()!=labels_size){
            throw runtime_error("Error labels sizes for "+cve_id+" should be: "+to_string(labels_size)+" but is: "+to_string(labels_map.size())+"\n");
        }
        labels.push_back(((int) labels_map["exploits_has"]));
    }else{
        throw runtime_error("Error unkown labels type for "+cve_id+": "+bsoncxx::to_string(labels_el.type())+"\n");
    }
    return pair<string,pair<vector<int>, vector<float>>>(cve_id,pair<vector<int>, vector<float>>(labels,features));
}

pair<string,pair<vector<int>, vector<float>>> MongoDB::bsonToDatasetEntry(bsoncxx::stdx::optional<bsoncxx::document::value> opt_bson){
    if(!opt_bson) {
        throw runtime_error("Empty opt result");
    }
    return bsonToDatasetEntry(opt_bson->view());
}

pair<vector<string>,vector<pair<vector<int>, vector<float>>>> MongoDB::loadCvesFromYear(int year, int limit){
    bsoncxx::document::value query = document{}
        << "cve"<< open_document
            << "$regex" <<  "CVE-"+to_string(year)+"-.*"
        << close_document
    << finalize;
    mongocxx::options::find opts{};
    opts.sort(document{}<<"cve"<<1<<finalize);
    mongocxx::cursor cursor = getCollection(getDB("processed_data"),"dataset").find(query.view(),opts);
    int total=0;
    vector<string> cves;
    vector<pair<vector<int>, vector<float>>> data;
    for(auto&& doc : cursor) {
        pair<string,pair<vector<int>, vector<float>>> parsed_entry=bsonToDatasetEntry(doc);
        cves.push_back(parsed_entry.first);
        data.push_back(parsed_entry.second);
        if (++total>=limit && limit>0){
            break;
        }
    }
    return pair<vector<string>,vector<pair<vector<int>, vector<float>>>>(cves,data);
}

pair<vector<string>,vector<pair<vector<int>, vector<float>>>> MongoDB::loadCvesFromYears(vector<int> years, int limit){
    vector<string> cves;
    vector<pair<vector<int>, vector<float>>> data;
    for(int y:years){
        pair<vector<string>,vector<pair<vector<int>, vector<float>>>> parsed_entries=loadCvesFromYear(y,limit);
        cves.insert(cves.end(),parsed_entries.first.begin(),parsed_entries.first.end());
        data.insert(data.end(),parsed_entries.second.begin(),parsed_entries.second.end());
    }
    return pair<vector<string>,vector<pair<vector<int>, vector<float>>>>(cves,data);
}

pair<vector<string>,vector<float>> MongoDB::fetchGeneticSimulationData(string id){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result=getCollection(getDB("genetic_db"),"simulations").find_one(document{} << "_id" << bsoncxx::oid{id} << finalize);
    string env_name;
    string train_data;
    string hall_of_fame_id;
    string population_id;
    float pop_start_size;
    float max_gens;
    float max_age;
    float max_children;
    float mutation_rate;
    float recycle_rate;
    float sex_rate;
    float max_notables;
    float cross_validation;
    float metric;
    float limit=0;
    float algorithm;
    if(maybe_result) {
        bsoncxx::document::element env_name_el = maybe_result->view()["env_name"];
        if (env_name_el.type() == bsoncxx::type::k_utf8){
            env_name=env_name_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(env_name_el.type())+"\n");
        }
        bsoncxx::document::element train_data_el = maybe_result->view()["train_data"];
        if (train_data_el.type() == bsoncxx::type::k_utf8){
            train_data=train_data_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(train_data_el.type())+"\n");
        }
        bsoncxx::document::element hall_of_fame_id_el = maybe_result->view()["hall_of_fame_id"];
        if (hall_of_fame_id_el.type() == bsoncxx::type::k_utf8){
            hall_of_fame_id=hall_of_fame_id_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(hall_of_fame_id_el.type())+"\n");
        }
        bsoncxx::document::element population_id_el = maybe_result->view()["population_id"];
        if (population_id_el.type() == bsoncxx::type::k_utf8){
            population_id=population_id_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(population_id_el.type())+"\n");
        }

        bsoncxx::document::element pop_start_size_el = maybe_result->view()["pop_start_size"];
        if (pop_start_size_el.type() == bsoncxx::type::k_int32){
            pop_start_size=(float)pop_start_size_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(pop_start_size_el.type())+"\n");
        }
        bsoncxx::document::element max_gens_el = maybe_result->view()["max_gens"];
        if (max_gens_el.type() == bsoncxx::type::k_int32){
            max_gens=(float)max_gens_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(max_gens_el.type())+"\n");
        }
        bsoncxx::document::element max_age_el = maybe_result->view()["max_age"];
        if (max_age_el.type() == bsoncxx::type::k_int32){
            max_age=(float)max_age_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(max_age_el.type())+"\n");
        }
        bsoncxx::document::element max_children_el = maybe_result->view()["max_children"];
        if (max_children_el.type() == bsoncxx::type::k_int32){
            max_children=(float)max_children_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(max_children_el.type())+"\n");
        }
        bsoncxx::document::element mutation_rate_el = maybe_result->view()["mutation_rate"];
        if (mutation_rate_el.type() == bsoncxx::type::k_double){
            mutation_rate=(float)mutation_rate_el.get_double();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(mutation_rate_el.type())+"\n");
        }
        bsoncxx::document::element recycle_rate_el = maybe_result->view()["recycle_rate"];
        if (recycle_rate_el.type() == bsoncxx::type::k_double){
            recycle_rate=(float)recycle_rate_el.get_double();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(recycle_rate_el.type())+"\n");
        }
        bsoncxx::document::element sex_rate_el = maybe_result->view()["sex_rate"];
        if (sex_rate_el.type() == bsoncxx::type::k_double){
            sex_rate=(float)sex_rate_el.get_double();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(sex_rate_el.type())+"\n");
        }
        bsoncxx::document::element max_notables_el = maybe_result->view()["max_notables"];
        if (max_notables_el.type() == bsoncxx::type::k_int32){
            max_notables=(float)max_notables_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(max_notables_el.type())+"\n");
        }
        bsoncxx::document::element cross_validation_el = maybe_result->view()["cross_validation"];
        if (cross_validation_el.type() == bsoncxx::type::k_int32){
            cross_validation=(float)cross_validation_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(cross_validation_el.type())+"\n");
        }
        bsoncxx::document::element metric_el = maybe_result->view()["metric"];
        if (metric_el.type() == bsoncxx::type::k_int32){
            metric=(float)metric_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(metric_el.type())+"\n");
        }
        bsoncxx::document::element algorithm_el = maybe_result->view()["algorithm"];
        if (algorithm_el.type() == bsoncxx::type::k_int32){
            algorithm=(float)algorithm_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(algorithm_el.type())+"\n");
        }
    }else{
        throw runtime_error("Unable to find simulation "+id);
    }
    string::size_type pos=train_data.find(':');
    if (pos!=string::npos){
        limit=stof(train_data.substr(pos+1,train_data.size()-pos-1));
        train_data=train_data.substr(0,pos);
    }
    vector<string> str_params;
    str_params.push_back(env_name);
    str_params.push_back(train_data);
    str_params.push_back(hall_of_fame_id);
    str_params.push_back(population_id);
    vector<float> float_params;
    float_params.push_back(pop_start_size);
    float_params.push_back(max_gens);
    float_params.push_back(max_age);
    float_params.push_back(max_children);
    float_params.push_back(mutation_rate);
    float_params.push_back(recycle_rate);
    float_params.push_back(sex_rate);
    float_params.push_back(max_notables);
    float_params.push_back(cross_validation);
    float_params.push_back(metric);
    float_params.push_back(limit);
    float_params.push_back(algorithm);

    return pair<vector<string>,vector<float>>(str_params,float_params);
}

void MongoDB::claimGeneticSimulation(string id,string currentDatetime, string hostname){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << "started_at" << open_document << "$ne" <<  NULL << close_document << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "started_at" <<  currentDatetime << close_document << finalize;
    getCollection(getDB("genetic_db"),"simulations").update_one(query.view(),update.view());

    query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    update=document{} << "$set" << open_document << "started_by" <<  hostname << close_document << finalize;
    getCollection(getDB("genetic_db"),"simulations").update_one(query.view(),update.view());
}

void MongoDB::finishGeneticSimulation(string id,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "finished_at" <<  currentDatetime << close_document << finalize;
    getCollection(getDB("genetic_db"),"simulations").update_one(query.view(),update.view());
}


void MongoDB::updateBestOnGeneticSimulation(string id, pair<float,int> candidate,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "updated_at" << currentDatetime << "best" <<  open_document << "output" <<  candidate.first << "at_gen" << candidate.second << close_document << close_document << finalize;
    getCollection(getDB("genetic_db"),"simulations").update_one(query.view(),update.view());   
}

void MongoDB::appendResultOnGeneticSimulation(string id, int pop_size,int g,float best_out,long timestamp_ms){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$push" << open_document << "results" <<  open_document << "pop_size" << pop_size << "cur_gen" <<  g << "gen_best_out" << best_out << "delta_ms" << timestamp_ms << close_document << close_document << finalize;
    getCollection(getDB("genetic_db"),"simulations").update_one(query.view(),update.view());
}

SPACE_SEARCH MongoDB::fetchEnvironmentData(string name){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result=getCollection(getDB("genetic_db"),"environments").find_one(document{} << "name" << name << finalize);
    INT_SPACE_SEARCH amount_of_layers;
    INT_SPACE_SEARCH epochs;
    INT_SPACE_SEARCH batch_size;
    INT_SPACE_SEARCH layer_size;
    INT_SPACE_SEARCH range_pow;
    INT_SPACE_SEARCH k_values;
    INT_SPACE_SEARCH l_values;
    INT_SPACE_SEARCH activation_funcs;
    FLOAT_SPACE_SEARCH alpha;
    FLOAT_SPACE_SEARCH sparcity;
    if(maybe_result) {
        bsoncxx::document::element space_search_el = maybe_result->view()["space_search"];
        if (space_search_el.type() == bsoncxx::type::k_document){
            bsoncxx::document::view space_search_vw=space_search_el.get_document().view();
            bsoncxx::document::view amount_of_layers_vw = space_search_vw["amount_of_layers"].get_document().view();
            bsoncxx::document::view epochs_vw = space_search_vw["epochs"].get_document().view();
            bsoncxx::document::view batch_size_vw = space_search_vw["batch_size"].get_document().view();
            bsoncxx::document::view layer_sizes_vw = space_search_vw["layer_sizes"].get_document().view();
            bsoncxx::document::view range_pow_vw = space_search_vw["range_pow"].get_document().view();
            bsoncxx::document::view K_vw = space_search_vw["K"].get_document().view();
            bsoncxx::document::view L_vw = space_search_vw["L"].get_document().view();
            bsoncxx::document::view activation_functions_vw = space_search_vw["activation_functions"].get_document().view();
            bsoncxx::document::view sparcity_vw = space_search_vw["sparcity"].get_document().view();
            bsoncxx::document::view alpha_vw = space_search_vw["alpha"].get_document().view();

            amount_of_layers = INT_SPACE_SEARCH(amount_of_layers_vw["min"].get_int32(),amount_of_layers_vw["max"].get_int32());
            epochs = INT_SPACE_SEARCH(epochs_vw["min"].get_int32(),epochs_vw["max"].get_int32());
            batch_size = INT_SPACE_SEARCH(batch_size_vw["min"].get_int32(),batch_size_vw["max"].get_int32());
            layer_size = INT_SPACE_SEARCH(layer_sizes_vw["min"].get_int32(),layer_sizes_vw["max"].get_int32());
            range_pow = INT_SPACE_SEARCH(range_pow_vw["min"].get_int32(),range_pow_vw["max"].get_int32());
            k_values = INT_SPACE_SEARCH(K_vw["min"].get_int32(),K_vw["max"].get_int32());
            l_values = INT_SPACE_SEARCH(L_vw["min"].get_int32(),L_vw["max"].get_int32());
            activation_funcs = INT_SPACE_SEARCH(activation_functions_vw["min"].get_int32(),activation_functions_vw["max"].get_int32());
            sparcity = FLOAT_SPACE_SEARCH((float)sparcity_vw["min"].get_double(),(float)sparcity_vw["max"].get_double());
            alpha = FLOAT_SPACE_SEARCH((float)alpha_vw["min"].get_double(),(float)alpha_vw["max"].get_double());
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(space_search_el.type())+"\n");
        }
    }
    return NeuralGenome::buildSlideNeuralNetworkSpaceSearch(amount_of_layers,epochs,alpha,batch_size,layer_size,range_pow,k_values,l_values,sparcity,activation_funcs);
}

void MongoDB::clearPopulationNeuralGenomeVector(string pop_id,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{pop_id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "updated_at" << currentDatetime << "neural_genomes" <<  open_array << close_array << close_document << finalize;
    getCollection(getDB("neural_db"),"populations").update_one(query.view(),update.view());
}

void MongoDB::addToPopulationNeuralGenomeVector(string pop_id,NeuralGenome* ng,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{pop_id} << finalize;
    bsoncxx::document::value update=document{} << "$push" << open_document << "neural_genomes" << castNeuralGenomeToBson(ng) << close_document << "$set" << open_document << "updated_at" << currentDatetime << close_document << finalize;
    getCollection(getDB("neural_db"),"populations").update_one(query.view(),update.view());
}

void MongoDB::clearHallOfFameNeuralGenomeVector(string hall_id,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{hall_id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "updated_at" << currentDatetime << "neural_genomes" <<  open_array << close_array << close_document << finalize;
    getCollection(getDB("neural_db"),"hall_of_fame").update_one(query.view(),update.view());
}

void MongoDB::addToHallOfFameNeuralGenomeVector(string hall_id,NeuralGenome* ng,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{hall_id} << finalize;
    bsoncxx::document::value update=document{} << "$push" << open_document << "neural_genomes" << castNeuralGenomeToBson(ng) << close_document << "$set" << open_document << "updated_at" << currentDatetime << close_document << finalize;
    getCollection(getDB("neural_db"),"hall_of_fame").update_one(query.view(),update.view());
}

bsoncxx::document::value MongoDB::castNeuralGenomeToBson(NeuralGenome* ng,bool store_weights){
    if (ng){
        pair<vector<int>,vector<float>> dna = ng->getDna();
        string int_dna="[ ";
        for(size_t i=0;i<dna.first.size();){
            int_dna+=to_string(dna.first[i]);
            if (++i<dna.first.size()-1){
                int_dna+=", ";
            }
        }
        int_dna+=" ]";
        string float_dna="[ ";
        for(size_t i=0;i<dna.second.size();){
            float_dna+=to_string(dna.second[i]);
            if (++i<dna.second.size()-1){
                float_dna+=", ";
            }
        }
        float_dna+=" ]";
        if (store_weights){
            bsoncxx::document::value full=document{}
            <<"int_dna"<<int_dna
            <<"float_dna"<<float_dna
            <<"output"<<ng->getOutput()
            <<"weights"<<Utils::serializeWeigthsToStr(ng->getWeights())
            << finalize;
            ng->clearWeightsIfCached();
            return full;
        }else{
            bsoncxx::document::value full=document{}
            <<"int_dna"<<int_dna
            <<"float_dna"<<float_dna
            <<"output"<<ng->getOutput()
            << finalize;
            return full;
        }
    }else{
        bsoncxx::document::value empty=document{} << finalize;
        return empty;
    }
}

pair<vector<string>,vector<int>> MongoDB::fetchNeuralNetworkTrainMetadata(string id){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result=getCollection(getDB("neural_db"),"independent_net").find_one(document{} << "_id" << bsoncxx::oid{id} << finalize);
    string hyper_name;
    string train_data;
    string test_data;
    int epochs;
    int cross_validation;
    int train_metric;
    int test_metric;
    int train_limit=0;
    int test_limit=0;
    if(maybe_result) {
        bsoncxx::document::element hyper_name_el = maybe_result->view()["hyperparameters_name"];
        if (hyper_name_el.type() == bsoncxx::type::k_utf8){
            hyper_name=hyper_name_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(hyper_name_el.type())+"\n");
        }
        bsoncxx::document::element train_data_el = maybe_result->view()["train_data"];
        if (train_data_el.type() == bsoncxx::type::k_utf8){
            train_data=train_data_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(train_data_el.type())+"\n");
        }
        bsoncxx::document::element test_data_el = maybe_result->view()["test_data"];
        if (test_data_el.type() == bsoncxx::type::k_utf8){
            test_data=test_data_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(test_data_el.type())+"\n");
        }
        bsoncxx::document::element epochs_el = maybe_result->view()["epochs"];
        if (epochs_el.type() == bsoncxx::type::k_int32){
            epochs=epochs_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(epochs_el.type())+"\n");
        }
        bsoncxx::document::element cross_validation_el = maybe_result->view()["cross_validation"];
        if (cross_validation_el.type() == bsoncxx::type::k_int32){
            cross_validation=cross_validation_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(cross_validation_el.type())+"\n");
        }
        bsoncxx::document::element train_metric_el = maybe_result->view()["train_metric"];
        if (train_metric_el.type() == bsoncxx::type::k_int32){
            train_metric=train_metric_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(train_metric_el.type())+"\n");
        }
        bsoncxx::document::element test_metric_el = maybe_result->view()["test_metric"];
        if (test_metric_el.type() == bsoncxx::type::k_int32){
            test_metric=test_metric_el.get_int32();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(test_metric_el.type())+"\n");
        }
    }else{
        throw runtime_error("Unable to find independent network "+id);
    }
    string::size_type pos=train_data.find(':');
    if (pos!=string::npos){
        train_limit=stof(train_data.substr(pos+1,train_data.size()-pos-1));
        train_data=train_data.substr(0,pos);
    }
    pos=test_data.find(':');
    if (pos!=string::npos){
        test_limit=stof(test_data.substr(pos+1,test_data.size()-pos-1));
        test_data=test_data.substr(0,pos);
    }

    vector<string> str_params;
    str_params.push_back(hyper_name);
    str_params.push_back(train_data);
    str_params.push_back(test_data);
    vector<int> int_params;
    int_params.push_back(epochs);
    int_params.push_back(cross_validation);
    int_params.push_back(train_metric);
    int_params.push_back(test_metric);
    int_params.push_back(train_limit);
    int_params.push_back(test_limit);

    return pair<vector<string>,vector<int>>(str_params,int_params);
}

void MongoDB::claimNeuralNetTrain(string id,string currentDatetime, string hostname){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << "started_at" << open_document << "$ne" <<  NULL << close_document << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "started_at" <<  currentDatetime << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());

    query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    update=document{} << "$set" << open_document << "started_by" <<  hostname << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());
}

void MongoDB::finishNeuralNetTrain(string id,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "finished_at" <<  currentDatetime << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());
}

Hyperparameters* MongoDB::fetchHyperparametersData(string name){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result=getCollection(getDB("neural_db"),"snn_hyperparameters").find_one(document{} << "name" << name << finalize);
    if(!maybe_result) {
        throw runtime_error("Hyperparameters not found!");
    }
    int layers=maybe_result->view()["layers"].get_int32();
    Hyperparameters* hyper=new Hyperparameters(layers);
    hyper->batch_size=maybe_result->view()["batch_size"].get_int32();
    hyper->alpha=(float)maybe_result->view()["alpha"].get_double();
    hyper->shuffle=maybe_result->view()["shuffle"].get_bool();
    hyper->adam=maybe_result->view()["adam"].get_bool();
    hyper->rehash=maybe_result->view()["rehash"].get_int32();
    hyper->rebuild=maybe_result->view()["rebuild"].get_int32();
    switch(maybe_result->view()["label_type"].get_int32()){
        case 0:
            hyper->label_type=SlideLabelEncoding::INT_CLASS;
            break;
        case 1:
            hyper->label_type=SlideLabelEncoding::NEURON_BY_NEURON;
            break;
        case 2:
            hyper->label_type=SlideLabelEncoding::NEURON_BY_N_LOG_LOSS;
            break;
    }
    bsoncxx::array::view layer_size_arr {maybe_result->view()["layer_sizes"].get_array().value};
    int idx=0;
    for (const bsoncxx::array::element& el:layer_size_arr){
        if(idx>=layers){
            break;
        }
        hyper->layer_sizes[idx]=el.get_int32();
        idx++;
    }
    bsoncxx::array::view range_pow_arr {maybe_result->view()["range_pow"].get_array().value};
    idx=0;
    for (const bsoncxx::array::element& el:range_pow_arr){
        if(idx>=layers){
            break;
        }
        hyper->range_pow[idx]=el.get_int32();
        idx++;
    }
    bsoncxx::array::view K_arr {maybe_result->view()["K"].get_array().value};
    idx=0;
    for (const bsoncxx::array::element& el:K_arr){
        if(idx>=layers){
            break;
        }
        hyper->K[idx]=el.get_int32();
        idx++;
    }
    bsoncxx::array::view L_arr {maybe_result->view()["L"].get_array().value};
    idx=0;
    for (const bsoncxx::array::element& el:L_arr){
        if(idx>=layers){
            break;
        }
        hyper->L[idx]=el.get_int32();
        idx++;
    }
    bsoncxx::array::view node_types_arr {maybe_result->view()["node_types"].get_array().value};
    idx=0;
    for (const bsoncxx::array::element& el:node_types_arr){
        if(idx>=layers){
            break;
        }
        switch(el.get_int32()){
            case 0:
                hyper->node_types[idx]=NodeType::ReLU;
                break;
            case 1:
                hyper->node_types[idx]=NodeType::Softmax;
                break;
            case 2:
                hyper->node_types[idx]=NodeType::Sigmoid;
                break;
        }
        idx++;
    }
    bsoncxx::array::view sparcity_arr {maybe_result->view()["sparcity"].get_array().value};
    idx=0;
    for (const bsoncxx::array::element& el:sparcity_arr){
        if(idx>=layers){
            break;
        }
        if (el.type() == bsoncxx::type::k_int32){
            hyper->sparcity[idx]=(float)el.get_int32();
        }else if (el.type() == bsoncxx::type::k_double){
            hyper->sparcity[idx]=(float)el.get_double();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(el.type())+"\n");
        }
        idx++;
    }
    return hyper;
}

void MongoDB::appendTMetricsOnNeuralNet(string id,vector<pair<float,float>> metrics){
    string metrics_str="[ ";
    for(size_t i=0;i<metrics.size();){
        if (metrics[i].second==-666){
            metrics_str+=to_string(metrics[i].first);
        }else{
            metrics_str+="{ "+to_string(metrics[i].first)+", "+to_string(metrics[i].second)+" }";
        }
        if (++i<metrics.size()){
            metrics_str+=", ";
        }
    }
    metrics_str+=" ]";
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "train_metrics" <<  metrics_str << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());
}

void MongoDB::appendStatsOnNeuralNet(string id,string field_name,snn_stats stats){
    bsoncxx::builder::basic::document stats_bson_builder{};
    stats_bson_builder.append(bsoncxx::builder::basic::kvp("accuracy",stats.accuracy));
    if (stats.precision!=-1)
        stats_bson_builder.append(bsoncxx::builder::basic::kvp("precision",stats.precision));
    if (stats.recall!=-1)
        stats_bson_builder.append(bsoncxx::builder::basic::kvp("recall",stats.recall));
    if (stats.f1!=-1)
        stats_bson_builder.append(bsoncxx::builder::basic::kvp("f1",stats.f1));
    bsoncxx::document::value stats_bson = stats_bson_builder.extract();
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << field_name <<  stats_bson << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());
}

void MongoDB::appendWeightsOnNeuralNet(string id,map<string, vector<float>> weights){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;
    bsoncxx::document::value update=document{} << "$set" << open_document << "weights" <<  Utils::serializeWeigthsToStr(weights) << close_document << finalize;
    getCollection(getDB("neural_db"),"independent_net").update_one(query.view(),update.view());
}

vector<pair<vector<int>, vector<float>>> MongoDB::loadCveFromId(string cve){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = getCollection(getDB("processed_data"),"dataset").find_one(document{} << "cve" << cve << finalize);
    vector<pair<vector<int>, vector<float>>> cve_parsed;
    cve_parsed.push_back(bsonToDatasetEntry(maybe_result).second);
    return cve_parsed;
}

map<string, vector<float>> MongoDB::loadWeightsFromNeuralNet(string id){
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result=getCollection(getDB("neural_db"),"independent_net").find_one(document{} << "_id" << bsoncxx::oid{id} << finalize);
    string weights_str="";
    if(maybe_result) {
        bsoncxx::document::element weights_el = maybe_result->view()["weights"];
        if (weights_el.type() == bsoncxx::type::k_utf8){
            weights_str=weights_el.get_utf8().value.data();
        }else{
            throw runtime_error("Error invalid type: "+bsoncxx::to_string(weights_el.type())+"\n");
        }
    }
    map<string, vector<float>> weights;
    if (weights_str!=""){
        weights=Utils::deserializeWeigthsFromStr(weights_str);
    }
    return weights;
}

void MongoDB::storeEvalNeuralNetResult(string id,int correct,vector<string> cve_ids,vector<vector<pair<int,float>>> pred_labels){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{id} << finalize;

    string predicted_labels_str="[\n";
    for (size_t i=0;i<cve_ids.size();i++){
        predicted_labels_str+="{ "+cve_ids[i]+": ";
        vector<pair<int,float>> label=pred_labels[i];
        predicted_labels_str+="{ has_exploit: "+to_string(label[0].first)+", trust_level: "+to_string(label[0].second)+" }\n";
    }
    predicted_labels_str+="\n]";

    bsoncxx::document::value update=document{} << "$set" << open_document << "total_test_cases" << (int)pred_labels.size() << "correct_predictions(not ground truth)" << correct << "predicted_labels" << predicted_labels_str << close_document << finalize;
    getCollection(getDB("neural_db"),"eval_results").update_one(query.view(),update.view());
}