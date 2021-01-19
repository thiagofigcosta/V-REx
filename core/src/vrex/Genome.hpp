#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>

#include "PopulationManager.hpp"
#include "Utils.hpp"

using namespace std;

class Genome{
    public:
        // constructors and destructor
        Genome(SPACE_SEARCH space, function<float(pair<vector<int>,vector<float>> dna)> callback);
        Genome(const Genome& orig, pair<vector<int>,vector<float>> new_dna);
        virtual ~Genome();
        void evaluate();

        // methods
        float getFitness();
        float getOutput();
        void setFitness(float nFit);
        void checkLimits();
        bool operator< (Genome& o);
        pair<vector<int>,vector<float>> getDna();
        void setDna(pair<vector<int>,vector<float>> new_dna);
        string to_string();
        boost::uuids::uuid getMtDna();
        void resetMtDna();

    private:
        // variables
        pair<vector<int>,vector<float>> dna;
        boost::uuids::uuid mt_dna;
        SPACE_SEARCH limits;
        float fitness;
        float output;
        function<float(pair<vector<int>,vector<float>> dna)> evaluate_cb; //float (*evaluate_cb)(pair<vector<int>,vector<float>> dna)
};