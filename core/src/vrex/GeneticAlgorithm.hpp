#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

using namespace std;

class GeneticAlgorithm{
    public:
        // destructor
        GeneticAlgorithm(){};
        virtual ~GeneticAlgorithm()=default;

        // methods
        virtual vector<Genome> select(vector<Genome> &currentGen)=0;
        virtual vector<Genome> fit(vector<Genome> &currentGen)=0;
        virtual vector<Genome> sex(Genome father, Genome mother)=0;
        virtual vector<Genome> mutate(vector<Genome> &individuals)=0;
        virtual unique_ptr<GeneticAlgorithm> clone()=0;
        void setLookingHighestFitness(bool highest){looking_highest_fitness=highest;};
    protected:
        bool looking_highest_fitness;
};