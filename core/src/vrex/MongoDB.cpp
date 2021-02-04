#include "MongoDB.hpp"

// http://mongocxx.org/mongocxx-v3/tutorial/
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

void MongoDB::addToHallOfFameNeuralGenomeVector(string hall_id,NeuralGenome* ng,string currentDatetime){
    bsoncxx::document::value query=document{} << "_id" << bsoncxx::oid{hall_id} << finalize;
    bsoncxx::document::value update=document{} << "$push" << open_document << "neural_genomes" << castNeuralGenomeToBson(ng) << close_document << "$set" << open_document << "updated_at" << currentDatetime << close_document << finalize;
    getCollection(getDB("neural_db"),"hall_of_fame").update_one(query.view(),update.view());
}

bsoncxx::document::value MongoDB::castNeuralGenomeToBson(NeuralGenome* ng,bool store_weights){
    // bsoncxx::builder::stream::document bson_stream_ng=document{};
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
            <<"weights"<<Utils::serializeWeigthsToStr(ng->getWeights())
            << finalize;
            ng->clearWeightsIfCached();
            return full;
        }else{
            bsoncxx::document::value full=document{}
            <<"int_dna"<<int_dna
            <<"float_dna"<<float_dna
            << finalize;
            return full;
        }
    }else{
        bsoncxx::document::value empty=document{} << finalize;
        return empty;
    }
}