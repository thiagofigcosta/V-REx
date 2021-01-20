#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <functional>

using namespace std;
class Genome;
class HallOfFame;

#include "GeneticAlgorithm.hpp"
#include "Genome.hpp"
#include "HallOfFame.hpp"
#include "EnchancedGenetic.hpp"


class PopulationManager{
    public:
        // constructors and destructor
        PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(Genome *self)> callback, int startPopulationSize, bool searchHighestFitness, bool useNeuralGenome=false);
        PopulationManager(const PopulationManager& orig);
        virtual ~PopulationManager();

        // methods
        void setHallOfFame(HallOfFame &hallOfFame);
        void naturalSelection(int gens);
        vector<Genome> getPopulation();
        static SPACE_SEARCH buildSlideNeuralNetworkSpaceSearch(INT_SPACE_SEARCH amount_of_layers,INT_SPACE_SEARCH epoachs,FLOAT_SPACE_SEARCH alpha,
                            INT_SPACE_SEARCH batch_size,INT_SPACE_SEARCH layer_size,INT_SPACE_SEARCH range_pow,INT_SPACE_SEARCH k_values,INT_SPACE_SEARCH l_values,
                            FLOAT_SPACE_SEARCH sparcity,INT_SPACE_SEARCH activation_funcs);

    private:
        // variables
        unique_ptr<GeneticAlgorithm> ga;
        vector<Genome> population;
        bool looking_highest_fitness;
        HallOfFame *hall_of_fame;
        static const int mt_dna_validity;
};