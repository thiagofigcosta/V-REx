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
        throw runtime_error("Error unkown type for CVE: "+bsoncxx::to_string(cve_el.type())+"\n");
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
                throw runtime_error("Error unkown type: "+bsoncxx::to_string(el.type())+"\n");
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
                throw runtime_error("Error unkown type: "+bsoncxx::to_string(el.type())+"\n");
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

pair<vector<string>,vector<pair<vector<int>, vector<float>>>> MongoDB::loadCvesFromYears(vector<int> years){
    vector<string> cves;
    vector<pair<vector<int>, vector<float>>> data;
    for(int y:years){
        pair<vector<string>,vector<pair<vector<int>, vector<float>>>> parsed_entries=loadCvesFromYear(y);
        cves.insert(cves.end(),parsed_entries.first.begin(),parsed_entries.first.end());
        data.insert(data.end(),parsed_entries.second.begin(),parsed_entries.second.end());
    }
    return pair<vector<string>,vector<pair<vector<int>, vector<float>>>>(cves,data);
}