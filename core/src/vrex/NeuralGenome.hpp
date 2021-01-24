#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>

#include "Genome.hpp"
#include "PopulationManager.hpp"
#include "Utils.hpp"
#include "Slide.hpp"

using namespace std;

class NeuralGenome : public Genome{
    public:
        // constructors and destructor
        NeuralGenome(SPACE_SEARCH space, function<float(Genome *self)> callback);
        NeuralGenome(const NeuralGenome& orig, pair<vector<int>,vector<float>> new_dna);
        NeuralGenome(const NeuralGenome& orig);
        virtual ~NeuralGenome();

        // methods
        map<string, vector<float>> getWeights();
        void setWeights(map<string, vector<float>> Weights);
        static SPACE_SEARCH buildSlideNeuralNetworkSpaceSearch(INT_SPACE_SEARCH amount_of_layers,INT_SPACE_SEARCH epochs,FLOAT_SPACE_SEARCH alpha,
                            INT_SPACE_SEARCH batch_size,INT_SPACE_SEARCH layer_size,INT_SPACE_SEARCH range_pow,INT_SPACE_SEARCH k_values,INT_SPACE_SEARCH l_values,
                            FLOAT_SPACE_SEARCH sparcity,INT_SPACE_SEARCH activation_funcs);
        static pair<Slide*,int> buildSlide(pair<vector<int>,vector<float>> dna, int input_size, int output_size, SlideLabelEncoding label_encoding, int rehash, int rebuild, int border_sparsity,  bool adam_optimizer=true);

    private:
        // variables
        map<string, vector<float>> weights;
};