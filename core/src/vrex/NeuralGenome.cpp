#include "NeuralGenome.hpp"

NeuralGenome::NeuralGenome(SPACE_SEARCH space, function<float(Genome *self)> callback)
    :Genome(space,callback){

}

NeuralGenome::NeuralGenome(const Genome& orig, pair<vector<int>,vector<float>> new_dna)
    :Genome(orig,new_dna){

}

NeuralGenome::~NeuralGenome(){

}