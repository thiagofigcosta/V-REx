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