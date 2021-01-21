#include "NeuralGenome.hpp"

NeuralGenome::NeuralGenome(SPACE_SEARCH space, function<float(Genome *self)> callback)
    :Genome(space,callback){

}

NeuralGenome::NeuralGenome(const NeuralGenome& orig, pair<vector<int>,vector<float>> new_dna)
    :Genome(orig,new_dna){
    weights=orig.weights;
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
}

NeuralGenome::~NeuralGenome(){

}

SPACE_SEARCH NeuralGenome::buildSlideNeuralNetworkSpaceSearch(INT_SPACE_SEARCH amount_of_layers,INT_SPACE_SEARCH epoachs,FLOAT_SPACE_SEARCH alpha,
                            INT_SPACE_SEARCH batch_size,INT_SPACE_SEARCH layer_size,INT_SPACE_SEARCH range_pow,INT_SPACE_SEARCH k_values,INT_SPACE_SEARCH l_values,
                            FLOAT_SPACE_SEARCH sparcity,INT_SPACE_SEARCH activation_funcs){
    vector<INT_SPACE_SEARCH> int_dna;
    vector<FLOAT_SPACE_SEARCH> float_dna;
    float_dna.push_back(alpha);
    int_dna.push_back(epoachs);
    int_dna.push_back(batch_size);
    int_dna.push_back(amount_of_layers);
    int max_layer_size=amount_of_layers.second;
    for(int i=0;i<max_layer_size;i++){
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
        float_dna.push_back(sparcity);
    }
    for(int i=0;i<max_layer_size-1;i++){
        int_dna.push_back(activation_funcs);
    }
    return SPACE_SEARCH(int_dna,float_dna);
}