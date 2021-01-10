#include "Slide.hpp"

Slide::Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, int *RangePow,
            int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, int Stepsize, SlideMode Mode,SlideHashingFunction HashFunc) {
        range_pow=RangePow;
        K=KValues;
        L=LValues;
        sparsity=Sparsity;
        batch_size=Batchsize;
        rehash=Rehash;
        rebuild=Rebuild;
        input_dim=InputDim;
        learning_rate=Lr;
        step_size=Stepsize;
        layer_sizes=sizesOfLayers;
        amount_layers=numLayer;
        slide_network=NULL;
        layer_types=layerTypes;
        mode=Mode;
        hash_function=HashFunc;

        slide_network=new Network(layer_sizes, layer_types, amount_layers, batch_size, learning_rate, input_dim, K, L, range_pow, sparsity,mode,hash_function);
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

vector<float> Slide::train(vector<pair<int, vector<float>>> train_data,int epochs){
    vector<pair<float,float>> losses = train(train_data,{},epochs);
    vector<float> log_loss;
    for (pair<float,float> ll:losses){
        log_loss.push_back(ll.first);
    }
    return log_loss;
}

vector<pair<float,float>> Slide::train(vector<pair<int, vector<float>>> train_data,vector<pair<int, vector<float>>> validation_data,int epochs){
    vector<pair<float,float>> log_losses; // train / validation
    int num_batches=(train_data.size() + batch_size-1)/batch_size;
    for (size_t j=0;j<(size_t)epochs;j++){
        float train_loss=0;
        for (size_t i = 0; i <(size_t)num_batches; i++) {
            int iter=j*num_batches+i;
            vector<pair<int, vector<float>>> batch_data=Utils::extractSubVector(train_data, i*batch_size, batch_size);
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
            
            int **records = new int *[batch_size];
            float **values = new float *[batch_size];
            int *sizes = new int[batch_size];
            int **labels = new int *[batch_size];
            int *labelsize = new int[batch_size];

            for (size_t c=0;c<(size_t)batch_size;c++){
                pair<int, vector<float>> entry=batch_data[c];
                float *value=&entry.second[0];
                int size=entry.second.size();
                int *record=new int[size];
                for (size_t t=0;t<entry.second.size();t++){
                    record[t]=1; // almost every input neuron is active???
                    if (entry.second[t]==0){
                        record[t]=0;
                    }
                }
                int label_size=1;// TODO make possible to have multiple labels
                int *label=new int[label_size];
                label[0]=entry.first;
                records[c]=record;
                values[c]=value;
                sizes[c]=size;
                labels[c]=label;
                labelsize[c]=label_size;
            }


            train_loss+=slide_network->ProcessInput(records, values, sizes, labels, labelsize, 
                                                    iter, must_rehash, must_rebuild);
            
            // clean up
            delete[] sizes;
            for (int d = 0; d < batch_size; d++){
                delete[] records[d];
                delete[] values[d];
                delete[] labels[d];
            }
            delete[] records;
            delete[] values;
            delete[] labels;
        }
        train_loss/=num_batches;
        cout << "Train loss: "<<train_loss<<endl;
        // TODO validation, use calcBackPropagateGrad to get the loss

        // TODO shuffle
    }
    return log_losses;
}