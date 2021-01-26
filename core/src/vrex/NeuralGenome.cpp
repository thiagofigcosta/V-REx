#include "NeuralGenome.hpp"

vector<pair<vector<int>, vector<float>>> NeuralGenome::static_train_data=vector<pair<vector<int>, vector<float>>> ();

NeuralGenome::NeuralGenome(SPACE_SEARCH space, function<float(Genome *self)> callback)
    :Genome(space,callback){
    train_data=vector<pair<vector<int>, vector<float>>>(NeuralGenome::static_train_data);
}

NeuralGenome::NeuralGenome(const NeuralGenome& orig, pair<vector<int>,vector<float>> new_dna)
    :Genome(orig,new_dna){
    weights=orig.weights;
    train_data=orig.train_data;
}

NeuralGenome::NeuralGenome(const NeuralGenome& orig){
    mt_dna=orig.mt_dna;
    dna=orig.dna;
    limits=orig.limits;
    fitness=orig.fitness;
    output=orig.output;
    evaluate_cb=orig.evaluate_cb;
    weights=orig.weights;
    id=orig.id;
    train_data=orig.train_data;
}

NeuralGenome::~NeuralGenome(){

}

SPACE_SEARCH NeuralGenome::buildSlideNeuralNetworkSpaceSearch(INT_SPACE_SEARCH amount_of_layers,INT_SPACE_SEARCH epochs,FLOAT_SPACE_SEARCH alpha,
                            INT_SPACE_SEARCH batch_size,INT_SPACE_SEARCH layer_size,INT_SPACE_SEARCH range_pow,INT_SPACE_SEARCH k_values,INT_SPACE_SEARCH l_values,
                            FLOAT_SPACE_SEARCH sparcity,INT_SPACE_SEARCH activation_funcs){
    vector<INT_SPACE_SEARCH> int_dna;
    vector<FLOAT_SPACE_SEARCH> float_dna;
    int_dna.push_back(epochs);
    int_dna.push_back(batch_size);
    int_dna.push_back(amount_of_layers);
    int max_layer_size=amount_of_layers.second;
    int_dna.push_back(INT_SPACE_SEARCH(max_layer_size,max_layer_size));
    for(int i=0;i<max_layer_size-1;i++){
        int_dna.push_back(layer_size);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(range_pow);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(k_values);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(l_values);
    }
    for(int i=0;i<max_layer_size-1;i++){
        int_dna.push_back(activation_funcs);
    }
    float_dna.push_back(alpha);
    for(int i=0;i<max_layer_size-2;i++){
        float_dna.push_back(sparcity);
    }
    return SPACE_SEARCH(int_dna,float_dna);
}

tuple<Slide*,int,function<void()>> NeuralGenome::buildSlide(pair<vector<int>,vector<float>> dna, int input_size, int output_size, SlideLabelEncoding label_encoding, int rehash, int rebuild, int border_sparsity, bool adam_optimizer){
    vector<int> int_dna=dna.first;
    vector<float> float_dna=dna.second;
    int epochs=int_dna[0];
    int batch_size=int_dna[1];
    int layers=int_dna[2];
    int max_layers=int_dna[3];

    int *layer_sizes=new int[layers];
    int *range_pow=new int[layers];
    int *K=new int[layers];
    int *L=new int[layers];
    NodeType* node_types = new NodeType[layers];
    float *sparcity=new float[layers];

    auto teardown_callback = [layer_sizes,range_pow,K,L,node_types,sparcity]() {
        // delete[] layer_sizes; // double free
        delete[] range_pow;
        delete[] K;
        delete[] L;
        // delete[] node_types; // double free
        delete[] sparcity;
    };
    
    sparcity[0]=border_sparsity;
    sparcity[layers-1]=border_sparsity;
    layer_sizes[layers-1]=output_size;
    node_types[layers-1]=NodeType::Softmax;

    int l=4;
    int i;
    for(i=0;i<max_layers-1;l++,i++){
        if (i+1<layers){
            layer_sizes[i]=int_dna[l];
        }
    }
    for(i=0;i<max_layers;l++,i++){
        if (i<layers){
            range_pow[i]=int_dna[l];
        }
    }
    for(i=0;i<max_layers;l++,i++){
        if (i<layers){
            int dna_value=int_dna[l]; 
            if (dna_value>layer_sizes[i]){  // ??? K cannot be higher than layer size?
                dna_value=layer_sizes[i];
            }
            K[i]=dna_value;
        }
    }
    for(i=0;i<max_layers;l++,i++){
        if (i<layers){
            L[i]=int_dna[l];
        }
    }
    for(i=0;i<max_layers-1;l++,i++){
        if (i+1<layers){
            switch(int_dna[l]){
                case 0:
                    node_types[i]=NodeType::Softmax;
                case 1:
                    node_types[i]=NodeType::ReLU;
                case 2:
                    node_types[i]=NodeType::Sigmoid;
            }
        }
    }

    float alpha=float_dna[0];
    l=1;
    for(i=1;i<max_layers-2;l++,i++){
        if (i+1<layers){
            sparcity[i]=float_dna[l];
        }
    }

    // string print_str;
    // for(i=0;i<layers;i++){
    //     print_str="layer_sizes["+std::to_string(i)+"]: "+std::to_string(layer_sizes[i])+"\n";
    //     cout<<print_str;
    //     print_str="range_pow["+std::to_string(i)+"]: "+std::to_string(range_pow[i])+"\n";
    //     cout<<print_str;
    //     print_str="K["+std::to_string(i)+"]: "+std::to_string(K[i])+"\n";
    //     cout<<print_str;
    //     print_str="L["+std::to_string(i)+"]: "+std::to_string(L[i])+"\n";
    //     cout<<print_str;
    //     print_str="node_types["+std::to_string(i)+"]: "+std::to_string(static_cast<underlying_type<NodeType>::type>(node_types[i]))+"\n";
    //     cout<<print_str;
    //     print_str="sparcity["+std::to_string(i)+"]: "+std::to_string(sparcity[i])+"\n";
    //     cout<<print_str;
    // }
    
    return {new Slide(layers, layer_sizes, node_types, input_size, alpha, batch_size, adam_optimizer, label_encoding,
    range_pow, K, L, sparcity, rehash, rebuild, SlideMode::SAMPLING, SlideHashingFunction::DENSIFIED_WTA, false),epochs,teardown_callback};
}

map<string, vector<float>> NeuralGenome::getWeights(){
    return weights;
}

void NeuralGenome::setWeights(map<string, vector<float>> Weights){
    weights=Weights;
}

void NeuralGenome::setNeuralTrainData(vector<pair<vector<int>, vector<float>>> data){
    NeuralGenome::static_train_data=data;
}

vector<pair<vector<int>, vector<float>>>& NeuralGenome::getTrainData(){
    return train_data;
}