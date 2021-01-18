#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>

using namespace std;
class Genome;
class HallOfFame;

typedef pair<int,int> INT_SPACE_SEARCH;
typedef pair<float,float> FLOAT_SPACE_SEARCH;
typedef pair<vector<INT_SPACE_SEARCH>,vector<FLOAT_SPACE_SEARCH>> SPACE_SEARCH;

#include "Genome.hpp"
#include "GeneticAlgorithm.hpp"
#include "HallOfFame.hpp"


class PopulationManager{
    public:
        // constructors and destructor
        PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(pair<vector<int>,vector<float>> dna)> callback, int startPopulationSize, bool searchHighestFitness);
        PopulationManager(const PopulationManager& orig);
        virtual ~PopulationManager();

        void setHallOfFame(HallOfFame &hallOfFame);
        void naturalSelection(int gens);
        vector<Genome> getPopulation();

    private:
        unique_ptr<GeneticAlgorithm> ga;
        vector<Genome> population;
        bool looking_highest_fitness;
        HallOfFame *hall_of_fame;
};