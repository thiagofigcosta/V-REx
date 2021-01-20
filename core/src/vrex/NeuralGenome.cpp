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
}

NeuralGenome::~NeuralGenome(){

}