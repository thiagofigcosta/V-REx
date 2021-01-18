#include "HallOfFame.hpp"

HallOfFame::HallOfFame(int maxNotables, bool lookingHighestFitness) {
    looking_highest_fitness=lookingHighestFitness;
    max_notables=maxNotables;
}

HallOfFame::HallOfFame(const HallOfFame& orig) {
}

HallOfFame::~HallOfFame() {
}

void HallOfFame::update(vector<Genome> candidates){
    vector<string> extras;
    update(candidates,extras);
}

void HallOfFame::update(vector<Genome> candidates, vector<string> extraCandidatesArguments){
    vector<pair<Genome,string>> candidatesWithExtra=joinGenomeVector(candidates,extraCandidatesArguments);
    for (pair<Genome,string> entry:candidatesWithExtra){
        notables.push_back(entry);
    }
    // notables.insert(notables.end(),candidatesWithExtra.begin(),candidatesWithExtra.end());
    sort(notables.begin(),notables.end(),[&](pair<Genome,string> &lhs, pair<Genome,string> &rhs){
        return lhs.first.getFitness() > rhs.first.getFitness(); // descending
    });
    // if (notables.size()>(size_t)max_notables){notables.resize(max_notables);}
    while(notables.size()>(size_t)max_notables){
        notables.pop_back();
    }
}

vector<Genome> HallOfFame::getNotables(){
    return splitGenomeVector(notables);
}

vector<pair<Genome,string>> HallOfFame::getNotablesPlusExtras(){
    return notables;
}

vector<pair<Genome,string>> HallOfFame::joinGenomeVector(vector<Genome> candidates, vector<string> extraCandidatesArguments){
    vector<pair<Genome,string>> out;
    for (size_t i=0;i<candidates.size();i++) {
        Genome gen=candidates[i];
        string extra="";
        if (i<extraCandidatesArguments.size()){
            extra=extraCandidatesArguments[i];
        }
        out.push_back(pair<Genome,string>(gen,extra));
    }
    return out;
}

vector<Genome> HallOfFame::splitGenomeVector(vector<pair<Genome,string>> genomeVec){
    vector<Genome> out;
    for (pair<Genome,string> notable:genomeVec){
        out.push_back(notable.first);
    }
    return out;
}