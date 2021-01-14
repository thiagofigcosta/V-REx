#include "test.hpp"

void testCsvRead(){
    cout << "Hello Thiago " << endl;

    vector<pair<string, vector<float>>> dataset= Utils::readLabeledCsvDataset(Utils::getResourcePath("iris.data"));
    for (pair<string, vector<float>> row : dataset) {
        cout << "------------" <<endl;
        cout << row.first <<endl;
        for (float el : row.second ){
            cout << el<< " " ;
        }
        cout <<endl;
        cout << "------------" <<endl;
    }
}

void testMongo(){
    string mongo_host;
    if (Utils::runningOnDockerContainer()){
        mongo_host="mongo";
    }else{
        mongo_host="127.0.0.1";
    }
    MongoDB mongo = MongoDB(mongo_host,"root","123456");
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        mongo.getCollection(mongo.getDB("processed_data"),"dataset").find_one(document{} << "cve" << "CVE-2017-0144" << finalize);
    if(maybe_result) {
        cout << "Eternal blue" <<endl << bsoncxx::to_json(*maybe_result) << endl;
    }

    mongocxx::cursor cursor = mongo.getCollection(mongo.getDB("processed_data"),"dataset").find({});
    int total=0;
    for(auto&& doc : cursor) {
        if (total++ == 0){
            string str_doc= bsoncxx::to_json(*maybe_result);
            bsoncxx::document::element cve = doc["cve"];
            cout << "first cve: "<< cve.get_utf8().value <<endl;
        }
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

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
    mongo.getCollection(mongo.getDB("tests"),"cxx").insert_one(view);
}

void testSlide(){
    bool print_data=false;
    pair<vector<pair<int, vector<float>>>,map<string,int>> enumfied = Utils::enumfyDataset(Utils::readLabeledCsvDataset(Utils::getResourcePath("iris.data")));
    vector<pair<vector<int>, vector<float>>> dataset = Utils::encodeDatasetLabels(enumfied.first,DataEncoder::INCREMENTAL);
    enumfied.first.clear(); // free
    if (print_data){
        cout << "Raw encoded data:"<<endl;
        for(pair<vector<int>, vector<float>> data:dataset){
            for (int el : data.first){
                cout<<el;
            }
            cout<<": ";
            for (float el : data.second){
                cout<<el<<",";
            }
            cout << endl;
        }
        cout<<"----------------------------"<<endl;
    }
    map<string,int> equivalence = enumfied.second;
    dataset=Utils::shuffleDataset(dataset);
    if (print_data){
        cout << "Raw encoded randomized data:"<<endl;
        for(pair<vector<int>, vector<float>> data:dataset){
            for (int el : data.first){
                cout<<el;
            }
            cout<<": ";
            for (float el : data.second){
                cout<<el<<",";
            }
            cout << endl;
        }
        cout<<"----------------------------"<<endl;
    }

    float train_percentage=.7;
    pair<vector<pair<vector<int>, vector<float>>>,vector<pair<vector<int>, vector<float>>>> dividedData=
        Utils::divideDataSet(dataset, train_percentage);
    vector<pair<vector<int>, vector<float>>> train_data=dividedData.first;
    vector<pair<vector<int>, vector<float>>> test_data=dividedData.second;
    if (print_data){
        cout << "Raw encoded splitted randomized data:"<<endl;
        cout << "Train data:"<<endl;
        for(pair<vector<int>, vector<float>> data:train_data){
            for (int el : data.first){
                cout<<el;
            }
            cout<<": ";
            for (float el : data.second){
                cout<<el<<",";
            }
            cout << endl;
        }
        cout<<endl<< "Test data:"<<endl;
        for(pair<vector<int>, vector<float>> data:test_data){
            for (int el : data.first){
                cout<<el;
            }
            cout<<": ";
            for (float el : data.second){
                cout<<el<<",";
            }
            cout << endl;
        }
        cout<<"----------------------------"<<endl;
    }

    // Too heavy for my computer :/
    // int layers=2;
    // int epochs=3;
    // float alpha=0.01;
    // int batch_size=5;
    // int *layer_sizes=new int[layers]{6,(int)train_data[0].first.size()};
    // int *range_pow=new int[layers]{6,18};
    // int *K=new int[layers]{2,6};
    // int *L=new int[layers]{20,50};
    // float *sparcity=new float[layers]{1,1};
    // int rehash=6400;
    // int rebuild=128000;
    // int step_size=10;

    int layers=1;
    int epochs=5;
    float alpha=0.01;
    int batch_size=5;
    int *layer_sizes=new int[layers]{(int)train_data[0].first.size()};
    int *range_pow=new int[layers]{6};
    int *K=new int[layers]{2};
    int *L=new int[layers]{20};
    float *sparcity=new float[layers]{1};
    int rehash=6400;
    int rebuild=128000;
    int step_size=10;
    bool print_deltas=true;
    Slide slide=Slide(layers, layer_sizes, Slide::getStdLayerTypes(layers), train_data[0].second.size(), alpha, batch_size, range_pow,
            K,L,sparcity, rehash, rebuild, step_size,SlideMode::SAMPLING,SlideHashingFunction::DENSIFIED_WTA,print_deltas);
    vector<float>train_losses=slide.train(train_data,epochs);
    for (float loss:train_losses){
        cout<<"Train loss: "<<loss<<endl;
    }
    float test_loss=slide.evalLoss(test_data);
    cout<<"Test loss: "<<test_loss<<endl;

    pair<int,vector<vector<pair<int,float>>>> predicted = slide.evalData(test_data);
    cout<<"Test size: "<<test_data.size()<<endl;
    cout<<"Correct values: "<<predicted.first<<endl;
    if (print_data){
        cout<<"Predicted values"<<endl;
        for (size_t i=0;i<predicted.second.size();i++){
            for (pair<int,float> el : predicted.second[i]){
                cout<<el.first;
            }
            cout<<"-> ";
            for (int el : test_data[i].first){
                cout<<el;
            }
            cout << endl;
        }
    }
}

void test() {
    // testCsvRead();
    // testMongo();
    testSlide();
}