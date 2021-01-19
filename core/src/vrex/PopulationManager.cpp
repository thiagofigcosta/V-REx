#include "PopulationManager.hpp"

PopulationManager::PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(pair<vector<int>,vector<float>> dna)> callback,int startPopulationSize, bool searchHighestFitness){
    ga=galg.clone();
    looking_highest_fitness=searchHighestFitness;
    if (typeid(*ga) == typeid(EnchancedGenetic)){
        EnchancedGenetic* casted_ga=dynamic_cast<EnchancedGenetic*>(ga.get());
        casted_ga->setMaxPopulation(startPopulationSize*2);
        casted_ga->enrichSpace(space);
    }else{
        space=ga->enrichSpace(space);
    }
    for (int i=0;i<startPopulationSize;i++){
        population.push_back(Genome(space,callback));
    }
    hall_of_fame=NULL;
}

PopulationManager::PopulationManager(const PopulationManager& orig) {
}

PopulationManager::~PopulationManager() {
    ga.release();
}

void PopulationManager::setHallOfFame(HallOfFame &hallOfFame){
    hall_of_fame=&hallOfFame;
}

const int PopulationManager::mt_dna_validity=15;

void PopulationManager::naturalSelection(int gens){
    ga->setLookingHighestFitness(looking_highest_fitness);
    for (int g=0;g<gens;){
        for(Genome &individual:population){ // evaluate output
            individual.evaluate();
        }
        population=ga->fit(population); // calculate fitness
        if (hall_of_fame!=NULL){
            hall_of_fame->update(population,g); // store best on hall of fame
        }
        if (++g<gens){
            population=ga->select(population); // selection + sex
            population=ga->mutate(population);  // mutation + age if aged algorithm
            if (g>0&&g%mt_dna_validity==0){
                for(Genome &individual:population){
                    individual.resetMtDna();
                }
            }
        }else{
            sort(population.begin(),population.end());
        }
    }
}

vector<Genome> PopulationManager::getPopulation(){
    return population;
}