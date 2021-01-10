#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace std;

class MongoDB{
    public:
        // constructors and destructor
        MongoDB(string host, string user, string password, int port=27017);
        MongoDB(const MongoDB& orig);
        virtual ~MongoDB();

        // methods
        mongocxx::database getDB(const string &db_name);
        mongocxx::collection getCollection(const mongocxx::database &db, const string &col_name);

    private:
        // methods
        static mongocxx::client getClient(const string &conn_str);
        // variables
        mongocxx::client client;

};