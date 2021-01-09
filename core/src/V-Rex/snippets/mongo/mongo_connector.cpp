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

mongocxx::client setup(string host, string user, string password){
    // to fix mongo driver if needed
    // sudo ln -s /usr/local/lib/libmogocxx.so.3.4.0 libmongocxx.so._noabi
    // sudo ln -s /usr/local/lib/libmongocxx.so._noabi /usr/local/lib/libmongocxx.so
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client client{mongocxx::uri{"mongodb://"+user+":"+password+"@"+host+"/?authSource=admin"}};

    return client;
}

// http://mongocxx.org/mongocxx-v3/tutorial/
int main (){
    mongocxx::client mongo = setup("127.0.0.1","root","123456");
    mongocxx::database db = mongo["processed_data"];
    mongocxx::collection coll = db["dataset"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        coll.find_one(document{} << "cve" << "CVE-2017-0144" << finalize);
    if(maybe_result) {
        cout << "Eternal blue" <<endl << bsoncxx::to_json(*maybe_result) << endl;
    }

    mongocxx::cursor cursor = coll.find({});
    int total=0;
    for(auto&& doc : cursor) {
        if (total++ == 0){
            bsoncxx::document::element cve = doc["cve"];
            cout << "first cve: "<< cve.get_utf8().value <<endl;
        }
        string str_doc= bsoncxx::to_json(*maybe_result);
    }
    cout << "Total documents " << total <<endl;


    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
    << "network_id" << "asd65as6d5a"
    << "layer_sizes" << bsoncxx::builder::stream::open_array
        << "500" << "2" << "3"
    << close_array
    << "weights" << bsoncxx::builder::stream::open_document
        << "layer1" <<  bsoncxx::builder::stream::open_array
            << "566" << "3453" << "53453"
        << close_array
        << "layer2" << bsoncxx::builder::stream::open_array
            << "566" << "3453" << "53453"
        << close_array
    << bsoncxx::builder::stream::close_document
    << bsoncxx::builder::stream::finalize;
    bsoncxx::document::view view = doc_value.view();

    mongocxx::database test_db = mongo["tests"];
    mongocxx::collection test_col = test_db["cxx"];

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
    test_col.insert_one(view);


}