#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

using namespace std;
typedef pair<int,int> INT_SPACE_SEARCH;
typedef pair<float,float> FLOAT_SPACE_SEARCH;
typedef pair<vector<INT_SPACE_SEARCH>,vector<FLOAT_SPACE_SEARCH>> SPACE_SEARCH;


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
        SPACE_SEARCH enrichSpace(SPACE_SEARCH &space){return space;};
    protected:
        bool looking_highest_fitness;
};