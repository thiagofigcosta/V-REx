#include "Slide.hpp"

const bool Slide::MEAN_ERROR_INSTEAD_OF_GRADS_SUM=true;

const float Slide::ADAM_OT_BETA1=0.9;
const float Slide::ADAM_OT_BETA2=0.999;
const float Slide::ADAM_OT_EPSILON=0.00000001;

const int Slide::BUCKET_SIZE=128;

const int Slide::TOPK_HASH_TOPK=30;
const int Slide::SIMHASH_RATIO=3;
const int Slide::WTA_BIN_SIZE=8; // binsize is the number of times the range is larger than the total number of hashes we need.

const int Slide::TOPK_THRESHOLD_SECONDS=2;

const bool Slide::FIFO_INSTEAD_OF_RESERVOIR_SAMPLING=true;

const float Slide::SOFTMAX_LINEAR_CONSTANT=0.0000001;

Slide::Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, bool useAdamOt, 
            SlideLabelEncoding labelType, int *RangePow, int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, 
            SlideMode Mode,SlideHashingFunction HashFunc, bool printDeltas) {
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

        slide_network=new Network(layer_sizes, layer_types, amount_layers, batch_size, learning_rate, input_dim, K, L, range_pow, sparsity,mode,hash_function,use_adam,label_type);
}

Slide::Slide(const Slide& orig) {
}

Slide::~Slide() {
    delete slide_network;
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

vector<float> Slide::train(vector<pair<vector<int>, vector<float>>> &train_data,int epochs){
    vector<pair<vector<int>, vector<float>>> validation_data;
    vector<pair<float,float>> losses = train(train_data,validation_data,epochs);
    vector<float> loss;
    for (pair<float,float> ll:losses){
        loss.push_back(ll.first);
    }
    return loss;
}

vector<pair<float,float>> Slide::train(vector<pair<vector<int>, vector<float>>> &train_data,vector<pair<vector<int>, vector<float>>> &validation_data,int epochs){
    vector<pair<float,float>> losses; // train / validation
    chrono::high_resolution_clock::time_point t1,t2;
    if (print_deltas) {
        t1 = chrono::high_resolution_clock::now();
    }
    // int num_batches=(train_data.size() + batch_size-1)/batch_size; // ERROR, round UP causes segfault
    int num_batches=train_data.size()/batch_size;
    for (size_t j=0;j<(size_t)epochs;j++){
        float train_loss=0;
        for (size_t i = 0; i <(size_t)num_batches; i++) {
            int iter=j*num_batches+i;
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

            batch_data.clear();
            deallocSlideDataset(values,sizes,records,labels,labelsize);
        }
        train_data=Utils::shuffleDataset(train_data);

        train_loss/=num_batches;

        float val_loss=0;
        if (validation_data.size()>0){
            val_loss=evalLoss(validation_data);
        }
        losses.push_back(pair<float,float>(train_loss,val_loss));
    }
    if (print_deltas) {
        t2 = chrono::high_resolution_clock::now();
        cout<<"Training takes: "<<Utils::msToHumanReadable(chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count())<<endl;
    }
    return losses;
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
}