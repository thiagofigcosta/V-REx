#include "HallOfFame.hpp"
#include "NeuralGenome.hpp"

HallOfFame::HallOfFame(int maxNotables, bool lookingHighestFitness, bool useExtraArgs) {
    looking_highest_fitness=lookingHighestFitness;
    max_notables=maxNotables;
    use_extra_args=useExtraArgs;
    float starting_point;
    if (looking_highest_fitness){
        starting_point=numeric_limits<float>::min();
    }else{
        starting_point=numeric_limits<float>::max();
    }
    best=pair<float,int>(starting_point,-1);
}

HallOfFame::HallOfFame(const HallOfFame& orig) {
}

HallOfFame::~HallOfFame() {
    for (Genome* g:notables){
        delete g;
    }
    for (pair<Genome*,string> g:notables_extra){
        delete g.first;
    }
}

void HallOfFame::update(vector<Genome*> candidates, int gen){
    if (!use_extra_args){
        for (Genome *entry:candidates){
            if (dynamic_cast<NeuralGenome*>(entry)){
                notables.push_back(new NeuralGenome(*((NeuralGenome*)entry)));
            }else{
                notables.push_back(new Genome(*entry));
            }
        }
        sort(notables.begin(),notables.end(),[&](Genome* &lhs, Genome* &rhs){
            if (looking_highest_fitness){
                return lhs->getOutput() > rhs->getOutput(); // descending
            }else{
                return lhs->getOutput() < rhs->getOutput(); // ascending
            }
        });
        while(notables.size()>(size_t)max_notables){
            notables.pop_back();
        }
        if ((looking_highest_fitness && notables[0]->getOutput()>best.first) || (!looking_highest_fitness && notables[0]->getOutput()<best.first)){
            best.first=notables[0]->getOutput();
            best.second=gen;
        }
    }else{
        vector<string> extras;
        update(candidates,extras,gen);
    }
}

void HallOfFame::update(vector<Genome*> candidates, vector<string> extraCandidatesArguments, int gen){
    vector<pair<Genome*,string>> candidatesWithExtra=joinGenomeVector(candidates,extraCandidatesArguments);
    for (pair<Genome*,string> entry:candidatesWithExtra){
        notables_extra.push_back(entry);
    }
    // notables.insert(notables.end(),candidatesWithExtra.begin(),candidatesWithExtra.end());
    sort(notables_extra.begin(),notables_extra.end(),[&](pair<Genome*,string> &lhs, pair<Genome*,string> &rhs){
        if (looking_highest_fitness){
            return lhs.first->getOutput() > rhs.first->getOutput(); // descending
        }else{
            return lhs.first->getOutput() < rhs.first->getOutput(); // ascending
        }
    });
    // if (notables.size()>(size_t)max_notables){notables.resize(max_notables);}
    while(notables_extra.size()>(size_t)max_notables){
        notables_extra.pop_back();
    }
    if ((looking_highest_fitness && notables_extra[0].first->getOutput()>best.first) || (!looking_highest_fitness && notables_extra[0].first->getOutput()<best.first)){
        best.first=notables_extra[0].first->getOutput();
        best.second=gen;
    }
}

vector<Genome*> HallOfFame::getNotables(){
    if (!use_extra_args){
        return notables;
    }else{
        return splitGenomeVector(notables_extra);
    }
}

vector<pair<Genome*,string>> HallOfFame::getNotablesPlusExtras(){
    if (!use_extra_args){
        return joinGenomeVector(notables,vector<string>());
    }else{
        return notables_extra;
    }
}
vector<Genome*> HallOfFame::splitGenomeVector(vector<pair<Genome*,string>> genomeVec){
    vector<Genome*> out;
    for (pair<Genome*,string> notable:genomeVec){
        out.push_back(notable.first);
    }
    return out;
}

pair<float,int> HallOfFame::getBest(){
    return best;
}

vector<pair<Genome*,string>> HallOfFame::joinGenomeVector(vector<Genome*> candidates, vector<string> extraCandidatesArguments){
    vector<pair<Genome*,string>> out;
    for (size_t i=0;i<candidates.size();i++) {
        Genome *gen=candidates[i];
        string extra="";
        if (i<extraCandidatesArguments.size()){
            extra=extraCandidatesArguments[i];
        }
        if (dynamic_cast<NeuralGenome*>(gen)){
            out.push_back(pair<Genome*,string>(new NeuralGenome(*((NeuralGenome*)gen)),extra));
        }else{
            out.push_back(pair<Genome*,string>(new Genome(*gen),extra));
        }
    }
    return out;
}