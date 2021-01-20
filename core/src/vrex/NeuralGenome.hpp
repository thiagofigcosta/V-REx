#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>

#include "Genome.hpp"
#include "PopulationManager.hpp"
#include "Utils.hpp"

using namespace std;

class NeuralGenome : public Genome{
    public:
        // constructors and destructor
        NeuralGenome(SPACE_SEARCH space, function<float(Genome *self)> callback);
        NeuralGenome(const NeuralGenome& orig, pair<vector<int>,vector<float>> new_dna);
        NeuralGenome(const NeuralGenome& orig);
        virtual ~NeuralGenome();

        // methods

        // variables
        map<string, vector<float>> weights;
};