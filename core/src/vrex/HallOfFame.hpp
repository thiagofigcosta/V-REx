#pragma once

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

#include "Genome.hpp"

class HallOfFame{
    public:
        // constructors and destructor
        HallOfFame(int maxNotables, bool lookingHighestFitness, bool useExtraArgs=false);
        HallOfFame(const HallOfFame& orig);
        virtual ~HallOfFame();

        // methods
        void update(vector<Genome*> candidates, int gen=-1);
        void update(vector<Genome*> candidates, vector<string> extraCandidatesArguments, int gen=-1); // store weights as extra
        vector<Genome*> getNotables();
        vector<pair<Genome*,string>> getNotablesPlusExtras();
        static vector<pair<Genome*,string>> joinGenomeVector(vector<Genome*> candidates, vector<string> extraCandidatesArguments);
        static vector<Genome*> splitGenomeVector(vector<pair<Genome*,string>> genomeVec);
        pair<float,int> getBest();
    private:
        // variables
        vector<pair<Genome*,string>> notables_extra;
        vector<Genome*> notables;
        pair<float,int> best;
        bool looking_highest_fitness;
        bool use_extra_args;
        int max_notables;
};