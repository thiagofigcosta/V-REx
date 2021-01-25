#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;
class Genome;
class HallOfFame;

#include "GeneticAlgorithm.hpp"
#include "EnchancedGenetic.hpp"


class PopulationManager{
    public:
        // constructors and destructor
        PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(Genome *self)> callback, int startPopulationSize, bool searchHighestFitness, bool useNeuralGenome=false, bool printDeltas=false);
        PopulationManager(const PopulationManager& orig);
        virtual ~PopulationManager();

        // methods
        void setHallOfFame(HallOfFame &hallOfFame);
        void naturalSelection(int gens);
        vector<Genome*> getPopulation();

    private:
        // variables
        unique_ptr<GeneticAlgorithm> ga;
        vector<Genome*> population;
        bool looking_highest_fitness;
        HallOfFame *hall_of_fame;
        bool print_deltas;
        static const int mt_dna_validity;
};