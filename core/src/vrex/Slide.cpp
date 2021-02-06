#include "Slide.hpp"
#include "slide/Node.h"
#include "slide/Network.h"
#include "Utils.hpp"


int Slide::MAX_THREADS=0; // 0 = max allowed 

Slide::Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, bool useAdamOt, 
            SlideLabelEncoding labelType, int *RangePow, int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, 
            SlideMetric trainMetric,SlideMetric valMetric,bool shuffleTrainData,SlideCrossValidation crossValidation,SlideMode Mode,SlideHashingFunction HashFunc, bool printDeltas) {
        range_pow=RangePow;
        K=KValues;
        L=LValues;
        sparsity=Sparsity;
        batch_size=Batchsize;
        rehash=Rehash;
        rebuild=Rebuild;
        input_dim=InputDim;
        learning_rate=Lr;
        layer_sizes=sizesOfLayers;
        amount_layers=numLayer;
        layer_types=layerTypes;
        mode=Mode;
        hash_function=HashFunc;
        print_deltas=printDeltas;
        use_adam=useAdamOt;
        label_type=labelType;
        train_metric=trainMetric;
        val_metric=valMetric;
        shuffle_train_data=shuffleTrainData;
        cross_validation=crossValidation;

        slide_network=new Network(layer_sizes, layer_types, amount_layers, batch_size, learning_rate, input_dim, K, L, range_pow, sparsity,mode,hash_function,use_adam,label_type);
}

Slide::Slide(const Slide& orig) {
}

Slide::~Slide() {
    delete slide_network;
    GarbageCollector::erase();
}

void Slide::setWeights(map<string, vector<float>> loadedData){
    slide_network->setWeights(loadedData);
}

map<string, vector<float>> Slide::getWeights(){
    return slide_network->mapfyWeights();
}

NodeType* Slide::getStdLayerTypes(const int amount_layers){
    NodeType* types = new NodeType[amount_layers];
    for (int i=0; i<amount_layers-1; i++){
        types[i] = NodeType::ReLU;
    }
    types[amount_layers-1] = NodeType::Softmax;
    return types;
}

vector<float> Slide::trainNoValidation(vector<pair<vector<int>, vector<float>>> &train_data,int epochs){
    SlideCrossValidation crossValidation = cross_validation;
    cross_validation=SlideCrossValidation::NONE;
    vector<pair<float,float>> losses = train(train_data,epochs);
    vector<float> loss;
    for (pair<float,float> ll:losses){
        loss.push_back(ll.first);
    }
    cross_validation=crossValidation;
    return loss;
}

pair<float,float> Slide::trainEpoch(vector<pair<vector<int>, vector<float>>> &train_data,vector<pair<vector<int>, vector<float>>> &validation_data, int cur_epoch){
    // int num_batches=(train_data.size() + batch_size-1)/batch_size; // ERROR, round UP causes segfault
    int num_batches=train_data.size()/batch_size;
    float train_loss=0;
    for (size_t i = 0; i <(size_t)num_batches; i++) {
        int iter=cur_epoch*num_batches+i;
        vector<pair<vector<int>, vector<float>>> batch_data=Utils::extractSubVector(train_data, i*batch_size, batch_size);
        bool must_rehash = false;
        bool must_rebuild = false;
        if ((size_t)(iter)%(rehash/batch_size) == ((size_t)rehash/batch_size-1)){
            if(mode==SlideMode::TOPK_THRESHOLD || mode==SlideMode::SAMPLING) {
                must_rehash = true;
            }
        }
        if ((size_t)(iter)%(rebuild/batch_size) == ((size_t)rehash/batch_size-1)){
            if(mode==SlideMode::TOPK_THRESHOLD || mode==SlideMode::SAMPLING) {
                must_rebuild = true;
            }
        }
        float **values;
        int *sizes, *labelsize;
        int **records, **labels;
        allocAndCastDatasetToSlide(batch_data,values,sizes,records,labels,labelsize);

        train_loss+=slide_network->ProcessInput(records, values, sizes, labels, labelsize, 
                                                iter, must_rehash, must_rebuild);

        for (pair<vector<int>, vector<float>> v:batch_data){
            v.first.clear();
            v.second.clear();
        }
        batch_data.clear();
        deallocSlideDataset(values,sizes,records,labels,labelsize);
    }
    train_loss/=num_batches;

    float train=0;
    switch(train_metric){
        case SlideMetric::RAW_LOSS:
            train=train_loss;
            break;
        case SlideMetric::F1:
            train=Utils::statisticalAnalysis(train_data,evalData(train_data).second).f1;
            break;
        case SlideMetric::RECALL:
            train=Utils::statisticalAnalysis(train_data,evalData(train_data).second).recall;
            break;
        case SlideMetric::ACCURACY:
            train=Utils::statisticalAnalysis(train_data,evalData(train_data).second).accuracy;
            break;
        case SlideMetric::PRECISION:
            train=Utils::statisticalAnalysis(train_data,evalData(train_data).second).precision;
            break;
    }

    float val=-666;
    if (validation_data.size()>0){
        switch(val_metric){
            case SlideMetric::RAW_LOSS:
                val=evalLoss(validation_data);
                break;
            case SlideMetric::F1:
                val=Utils::statisticalAnalysis(validation_data,evalData(validation_data).second).f1;
                break;
            case SlideMetric::RECALL:
                val=Utils::statisticalAnalysis(validation_data,evalData(validation_data).second).recall;
                break;
            case SlideMetric::ACCURACY:
                val=Utils::statisticalAnalysis(validation_data,evalData(validation_data).second).accuracy;
                break;
            case SlideMetric::PRECISION:
                val=Utils::statisticalAnalysis(validation_data,evalData(validation_data).second).precision;
                break;
        }
        GarbageCollector::get()->flush();
    }
    return pair<float,float>(train,val);
}

vector<pair<float,float>> Slide::train(vector<pair<vector<int>, vector<float>>> &train_data,int epochs){
    vector<pair<float,float>> metrics; // train / validation
    chrono::high_resolution_clock::time_point t1,t2;
    if (print_deltas) {
        t1 = chrono::high_resolution_clock::now();
    }
    for (size_t j=0;j<(size_t)epochs;j++){
        if(shuffle_train_data)
            train_data=Utils::shuffleDataset(train_data);
        vector<pair<vector<int>, vector<float>>> validation_data;
        vector<pair<vector<int>, vector<float>>> t_data;
        switch(cross_validation){
            case SlideCrossValidation::NONE:
                t_data=train_data;
                break;
            case SlideCrossValidation::ROLLING_FORECASTING_ORIGIN:{
                int fixed_train_pos=train_data.size()*Slide::ROLLING_FORECASTING_ORIGIN_MIN;
                int window_size=(train_data.size()-fixed_train_pos)/epochs;
                int start_val=fixed_train_pos+j*window_size;
                t_data=Utils::extractSubVector(train_data, 0, start_val);
                validation_data=Utils::extractSubVector(train_data, start_val, window_size);
                }break;
            case SlideCrossValidation::KFOLDS:{
                const int total_folds=Slide::K_FOLDS;
                int val_fold=int((total_folds-1)*Utils::getRandomBetweenZeroAndOne());
                int fold_size=train_data.size()/total_folds;
                t_data=Utils::extractSubVector(train_data, 0, val_fold*fold_size );
                validation_data=Utils::extractSubVector(train_data, val_fold*fold_size, fold_size );
                if (val_fold!=(total_folds-1)){
                    vector<pair<vector<int>, vector<float>>> tmp=Utils::extractSubVector(train_data, (val_fold+1)*fold_size, (total_folds-val_fold-1)*fold_size );
                    t_data.insert(t_data.end(),tmp.begin(),tmp.end());
                    tmp.clear();
                }
                }break;
            case SlideCrossValidation::TWENTY_PERCENT:{
                int val_size=.2*train_data.size();
                t_data=Utils::extractSubVector(train_data, 0, train_data.size()-val_size);
                validation_data=Utils::extractSubVector(train_data, train_data.size()-val_size, val_size);
                }break;
        }
        while((int)validation_data.size()<batch_size && cross_validation!=SlideCrossValidation::NONE){
            validation_data.push_back(t_data[int(t_data.size()*Utils::getRandomBetweenZeroAndOne())]);
        }
        metrics.push_back(trainEpoch(t_data,validation_data,j));
        for (pair<vector<int>, vector<float>> v:t_data){
            v.first.clear();
            v.second.clear();
        }
        t_data.clear();
        for (pair<vector<int>, vector<float>> v:validation_data){
            v.first.clear();
            v.second.clear();
        }
        validation_data.clear();
    }
    if (print_deltas) {
        t2 = chrono::high_resolution_clock::now();
        cout<<"Training takes: "<<Utils::msToHumanReadable(chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())<<endl;
    }
    return metrics;
}

float Slide::evalLoss(vector<pair<vector<int>, vector<float>>> &eval_data){
    // int num_batches=(eval_data.size() + batch_size-1)/batch_size; // ERROR, round UP causes segfault
    int num_batches=eval_data.size()/batch_size;
    float loss=0;
    chrono::high_resolution_clock::time_point t1,t2;
    if (print_deltas) {
        t1 = chrono::high_resolution_clock::now();
    }
    for (size_t i = 0; i <(size_t)num_batches; i++) {
        vector<pair<vector<int>, vector<float>>> batch_data=Utils::extractSubVector(eval_data, i*batch_size, batch_size);
        float **values;
        int *sizes, *labelsize;
        int **records, **labels;
        allocAndCastDatasetToSlide(batch_data,values,sizes,records,labels,labelsize);

        loss+=slide_network->evalInput(records, values, sizes, labels, labelsize);

        for (pair<vector<int>, vector<float>> v:batch_data){
            v.first.clear();
            v.second.clear();
        }
        batch_data.clear();
        deallocSlideDataset(values,sizes,records,labels,labelsize);
    }
    loss/=num_batches;
    if (print_deltas) {
        t2 = chrono::high_resolution_clock::now();
        cout<<"Loss evaluation takes: "<<Utils::msToHumanReadable(chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())<<endl;
    }
    return loss;
}

pair<int,vector<vector<pair<int,float>>>> Slide::evalData(vector<pair<vector<int>, vector<float>>> &test_data){
    pair<int,vector<vector<pair<int,float>>>>  output;
    chrono::high_resolution_clock::time_point t1,t2;
    if (print_deltas) {
        t1 = chrono::high_resolution_clock::now();
    }
    // int num_batches=(test_data.size() + batch_size-1)/batch_size; // ERROR, round UP causes segfault
    int num_batches=test_data.size()/batch_size;
    for (size_t i = 0; i <(size_t)num_batches; i++) {
        vector<pair<vector<int>, vector<float>>> batch_data=Utils::extractSubVector(test_data, i*batch_size, batch_size);
        float **values;
        int *sizes, *labelsize;
        int **records, **labels;
        allocAndCastDatasetToSlide(batch_data,values,sizes,records,labels,labelsize);
        pair<int,vector<vector<pair<int,float>>>> batch_out=slide_network->predictClass(records, values, sizes, labels, labelsize);
        output.second.insert(output.second.end(),batch_out.second.begin(),batch_out.second.end());
        output.first+=batch_out.first;
        
        for (pair<vector<int>, vector<float>> v:batch_data){
            v.first.clear();
            v.second.clear();
        }
        batch_data.clear();
        deallocSlideDataset(values,sizes,records,labels,labelsize);
    }
    if (print_deltas) {
        t2 = chrono::high_resolution_clock::now();
        cout<<"Inference takes: "<<Utils::msToHumanReadable(chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())<<endl;
    }
    return output;
}

void Slide::allocAndCastDatasetToSlide(vector<pair<vector<int>, vector<float>>> &data,float **&values, int *&sizes, int **&records, int **&labels, int *&labelsize){
    values = new float *[data.size()];
    sizes = new int[data.size()];
    records = new int *[data.size()];
    labels = new int *[data.size()];
    labelsize = new int[data.size()];
    for (size_t c=0;c<data.size();c++){
        pair<vector<int>, vector<float>> entry=data[c];
        sizes[c]=entry.second.size();
        labelsize[c]=entry.first.size();
        records[c]=new int[sizes[c]];
        values[c]=new float [sizes[c]];
        labels[c]=new int [labelsize[c]];
        for (size_t t=0;t<(size_t)sizes[c];t++){
            records[c][t]=1; // almost every input neuron is active???
            if (entry.second[t]==0){
                records[c][t]=0;
            }
            values[c][t]=entry.second[t];
        }
        for (size_t t=0;t<(size_t)labelsize[c];t++){
            labels[c][t]=entry.first[t];
        }
    }
}

void Slide::deallocSlideDataset(float **values, int *sizes, int **records, int **labels, int *labelsize){
    // clean up
    delete[] sizes;
    delete[] labelsize;
    for (int d = 0; d < batch_size; d++) { // causing exception: double free or corruption (fasttop) ???? not causing anymore?
        delete[] records[d]; 
        delete[] values[d];
        delete[] labels[d]; 
    }
    delete[] records;
    delete[] values;
    delete[] labels;
    GarbageCollector::get()->flush();
}
