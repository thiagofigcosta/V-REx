#pragma once

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

#include "Genome.hpp"

class HallOfFame{
    public:
        // constructors and destructor
        HallOfFame(int maxNotables, bool lookingHighestFitness);
        HallOfFame(const HallOfFame& orig);
        virtual ~HallOfFame();

        // methods
        void update(vector<Genome> candidates);
        void update(vector<Genome> candidates, vector<string> extraCandidatesArguments); // store weights as extra
        vector<Genome> getNotables();
        vector<pair<Genome,string>> getNotablesPlusExtras();
        static vector<pair<Genome,string>> joinGenomeVector(vector<Genome> candidates, vector<string> extraCandidatesArguments);
        static vector<Genome> splitGenomeVector(vector<pair<Genome,string>> genomeVec);
    private:
        // variables
        vector<pair<Genome,string>> notables;
        bool looking_highest_fitness;
        int max_notables;
};