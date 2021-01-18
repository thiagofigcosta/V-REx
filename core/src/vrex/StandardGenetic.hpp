#pragma once

#include <iostream>
#include <utility>
#include <vector>

using namespace std;
class Genome;
enum class StdGeneticRankType { RELATIVE, ABSOLUTE };

#include "GeneticAlgorithm.hpp"
#include "Genome.hpp"
#include "Utils.hpp"

class StandardGenetic : public GeneticAlgorithm{ 
    public:
        // constructors and destructor
        StandardGenetic(float mutationRate, float sexRate, StdGeneticRankType rankType);
        StandardGenetic(const StandardGenetic& orig);
        virtual ~StandardGenetic();

        //methods override
        vector<Genome> select(vector<Genome> &currentGen);
        vector<Genome> fit(vector<Genome> &currentGen);
        vector<Genome> sex(Genome father, Genome mother);
        vector<Genome> mutate(vector<Genome> &individuals);
        unique_ptr<GeneticAlgorithm> clone();

    private:
        // variables 
        float mutation_rate;
        float sex_rate;
        StdGeneticRankType rank_type;

        //methods 
        float genRandomFactor();
};