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

void testSlide_IntLabel(){
    cout<<"Using int label"<<endl;
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
    dataset=Utils::normalizeDataset(dataset).second;
    if (print_data){
        cout << "Raw encoded normalized data:"<<endl;
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
        cout << "Raw encoded normalized randomized data:"<<endl;
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
    // bool adam=true;
    // int *range_pow=new int[layers]{6,18};
    // int *K=new int[layers]{2,6};
    // int *L=new int[layers]{20,50};
    // float *sparcity=new float[layers]{1,1};
    // int rehash=6400;
    // int rebuild=128000;
    // bool print_deltas=true;

    int layers=1;
    int epochs=5;
    float alpha=0.01;
    int batch_size=5;
    SlideLabelEncoding label_type=SlideLabelEncoding::INT_CLASS;
    int *layer_sizes=new int[layers]{(int)train_data[0].first.size()};
    bool adam=true;
    int *range_pow=new int[layers]{6};
    int *K=new int[layers]{2};
    int *L=new int[layers]{20};
    float *sparcity=new float[layers]{1};
    int rehash=6400;
    int rebuild=128000;
    bool print_deltas=true;
    Slide slide=Slide(layers, layer_sizes, Slide::getStdLayerTypes(layers), train_data[0].second.size(), alpha, batch_size, adam, label_type,
    range_pow, K, L, sparcity, rehash, rebuild, SlideMode::SAMPLING, SlideHashingFunction::DENSIFIED_WTA, print_deltas);
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

void testSlide_NeuronByNeuronLabel(){
    cout<<endl<<"Using neuron label"<<endl;
    pair<vector<pair<int, vector<float>>>,map<string,int>> enumfied = Utils::enumfyDataset(Utils::readLabeledCsvDataset(Utils::getResourcePath("iris.data")));
    vector<pair<vector<int>, vector<float>>> dataset = Utils::encodeDatasetLabels(enumfied.first,DataEncoder::BINARY);
    // vector<pair<vector<int>, vector<float>>> dataset = Utils::encodeDatasetLabels(enumfied.first,DataEncoder::SPARSE);
    enumfied.first.clear(); // free
  
    dataset=Utils::shuffleDataset(dataset);
    dataset=Utils::normalizeDataset(dataset).second;

    float train_percentage=.7;
    pair<vector<pair<vector<int>, vector<float>>>,vector<pair<vector<int>, vector<float>>>> dividedData=
        Utils::divideDataSet(dataset, train_percentage);
    vector<pair<vector<int>, vector<float>>> train_data=dividedData.first;
    vector<pair<vector<int>, vector<float>>> test_data=dividedData.second;

    int layers=1;
    int epochs=150;
    float alpha=0.1;
    int batch_size=15;
    SlideLabelEncoding label_type=SlideLabelEncoding::NEURON_BY_NEURON;
    int *layer_sizes=new int[layers]{(int)train_data[0].first.size()};
    bool adam=true;
    int *range_pow=new int[layers]{6};
    int *K=new int[layers]{2};
    int *L=new int[layers]{20};
    float *sparcity=new float[layers]{1};
    int rehash=6400;
    int rebuild=128000;
    bool print_deltas=true;
    Slide slide=Slide(layers, layer_sizes, Slide::getStdLayerTypes(layers), train_data[0].second.size(), alpha, batch_size, adam, label_type,
    range_pow, K, L, sparcity, rehash, rebuild, SlideMode::SAMPLING, SlideHashingFunction::DENSIFIED_WTA, print_deltas);
    vector<float>train_losses=slide.train(train_data,epochs);
    float total_loss=0;
    for (float loss:train_losses){
       total_loss+=loss;
    }
    cout<<"Avg Train loss: "<<total_loss/train_losses.size()<<endl;
    // for (float loss:train_losses){
    //     cout<<"Train loss: "<<loss<<endl;
    // }
    float test_loss=slide.evalLoss(test_data);
    cout<<"Test loss: "<<test_loss<<endl;

    pair<int,vector<vector<pair<int,float>>>> predicted = slide.evalData(test_data);
    cout<<"Test size: "<<test_data.size()<<endl;
    cout<<"Correct values: "<<predicted.first<<endl;
}

void testStdGeneticsOnMath(){
    cout<<"Minimization:"<<endl;
    FLOAT_SPACE_SEARCH x = FLOAT_SPACE_SEARCH(-512,512);
    vector<FLOAT_SPACE_SEARCH> limits;
    limits.push_back(x);
    limits.push_back(x);
    SPACE_SEARCH space=SPACE_SEARCH(vector<INT_SPACE_SEARCH>(),limits);
    int population_size=300;
    int max_gens=100;
    float mutation_rate=0.2;
    float sex_rate=0.6;
    bool search_maximum=false;
    int max_notables=5;
    HallOfFame elite_min=HallOfFame(max_notables, search_maximum);
    StandardGenetic ga = StandardGenetic(mutation_rate, sex_rate);
    PopulationManager population=PopulationManager(ga, space, [](pair<vector<int>,vector<float>> dna) -> float {return
        // https://www.sfu.ca/~ssurjano/egg.html // minimum -> x1=512 | x2=404.2319 -> y(x1,x2)=-959.6407
        -(dna.second[1]+47)*sin(sqrt(abs(dna.second[1]+(dna.second[0]/2)+47)))-dna.second[0]*sin(sqrt(abs(dna.second[0]-(dna.second[1]+47))));}
        ,population_size, search_maximum);
    population.setHallOfFame(elite_min);
    population.naturalSelection(max_gens);
    for (Genome individual: elite_min.getNotables()){
        cout<<individual.to_string()<<endl;
    }
    cout<<"Expected: y(512,404.2319) = -959.6407"<<endl;
    cout<<endl<<endl;

    cout<<"Maximization:"<<endl;
    FLOAT_SPACE_SEARCH x2 = FLOAT_SPACE_SEARCH(-100,100);
    limits.clear();
    limits.push_back(x2);
    limits.push_back(x2);
    SPACE_SEARCH space2=SPACE_SEARCH(vector<INT_SPACE_SEARCH>(),limits);
    population_size=100;
    max_gens=100;
    mutation_rate=0.1;
    sex_rate=0.7;
    search_maximum=true;
    max_notables=5;
    HallOfFame elite_max=HallOfFame(max_notables, search_maximum);
    ga = StandardGenetic(mutation_rate, sex_rate);
    PopulationManager population2=PopulationManager(ga, space2, [](pair<vector<int>,vector<float>> dna) -> float {return
        // https://www.sfu.ca/~ssurjano/easom.html TIME MINUS ONE // maximum -> x1=x2=pi -> y(x1,x2)=1
        -(-cos(dna.second[0])*cos(dna.second[1])*exp(-(pow(dna.second[0]-M_PI,2)+pow(dna.second[1]-M_PI,2))));}
        ,population_size, search_maximum);
    population2.setHallOfFame(elite_max);
    population2.naturalSelection(max_gens);
    for (Genome individual: elite_max.getNotables()){
        cout<<individual.to_string()<<endl;
    }
    cout<<"Expected: y(3.141592,3.141592) = 1"<<endl;
}

void test() {
    // testCsvRead();
    // testMongo();
    // testSlide_IntLabel();
    // testSlide_NeuronByNeuronLabel();
    testStdGeneticsOnMath();
}