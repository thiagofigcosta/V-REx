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

#include "Utils.hpp"
#include "NeuralGenome.hpp"
#include "GeneticAlgorithm.hpp"

class MongoDB{
    public:
        // constructors and destructor
        MongoDB(string host, string user, string password, int port=27017);
        MongoDB(const MongoDB& orig);
        virtual ~MongoDB();

        // methods
        mongocxx::database getDB(const string &db_name);
        mongocxx::collection getCollection(const mongocxx::database &db, const string &col_name);
        static pair<string,pair<vector<int>, vector<float>>> bsonToDatasetEntry(const bsoncxx::v_noabi::document::view bson);
        static pair<string,pair<vector<int>, vector<float>>> bsonToDatasetEntry(bsoncxx::stdx::optional<bsoncxx::document::value> opt_bson);
        pair<vector<string>,vector<pair<vector<int>, vector<float>>>> loadCvesFromYear(int year, int limit=0);
        pair<vector<string>,vector<pair<vector<int>, vector<float>>>> loadCvesFromYears(vector<int> years, int limit=0);
        pair<vector<string>,vector<float>> fetchGeneticSimulationData(string id);
        void claimGeneticSimulation(string id,string currentDatetime, string hostname);
        void finishGeneticSimulation(string id,string currentDatetime);
        void updateBestOnGeneticSimulation(string id, pair<float,int> candidate,string currentDatetime);
        void appendResultOnGeneticSimulation(string id,int pop_size,int g,float best_out,long timestamp_ms);
        SPACE_SEARCH fetchEnvironmentData(string name);
        void clearPopulationNeuralGenomeVector(string pop_id,string currentDatetime);
        void addToPopulationNeuralGenomeVector(string pop_id,NeuralGenome* ng,string currentDatetime);
        void addToHallOfFameNeuralGenomeVector(string hall_id,NeuralGenome* ng,string currentDatetime);

    private:
        // methods
        static mongocxx::client getClient(const string &conn_str);
        bsoncxx::document::value castNeuralGenomeToBson(NeuralGenome* ng);
        // variables
        mongocxx::client client;

}; 