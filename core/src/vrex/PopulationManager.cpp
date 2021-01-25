#include "PopulationManager.hpp"
#include "NeuralGenome.hpp"
#include "HallOfFame.hpp"

PopulationManager::PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(Genome *self)> callback,int startPopulationSize, bool searchHighestFitness, bool useNeuralGenome, bool printDeltas){
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
        if (useNeuralGenome){
            population.push_back(new NeuralGenome(space,callback));
        }else{
            population.push_back(new Genome(space,callback));
        }
    }
    print_deltas=printDeltas;
    hall_of_fame=nullptr;
}

PopulationManager::PopulationManager(const PopulationManager& orig) {
}

PopulationManager::~PopulationManager() {
    GeneticAlgorithm* ptr=ga.release();
    ga.get_deleter() ( ptr );
    for (Genome* g:population){
        delete g;
    }
}

void PopulationManager::setHallOfFame(HallOfFame &hallOfFame){
    hall_of_fame=&hallOfFame;
}

const int PopulationManager::mt_dna_validity=15;

void PopulationManager::naturalSelection(int gens){
    ga->setLookingHighestFitness(looking_highest_fitness);
    chrono::high_resolution_clock::time_point t1,t2;
    for (int g=0;g<gens;){
        if (print_deltas) {
            t1 = chrono::high_resolution_clock::now();
        }
        for(Genome *individual:population){ // evaluate output
            individual->evaluate();
        }
        ga->fit(population); // calculate fitness
        if (hall_of_fame){
            hall_of_fame->update(population,g); // store best on hall of fame
        }
        if (++g<gens){
            ga->select(population); // selection + sex
            ga->mutate(population);  // mutation + age if aged algorithm
            if (g>0&&g%mt_dna_validity==0){
                for(Genome *individual:population){
                    individual->resetMtDna();
                }
            }
        }else{
            sort(population.begin(),population.end(),Genome::compare);
        }
        if (print_deltas) {
            t2 = chrono::high_resolution_clock::now();
            cout<<"Generation "<<(g+1)<<" of "<<gens<<" takes: "<<chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()<<" ms"<<endl;
        }
    }
}

vector<Genome*> PopulationManager::getPopulation(){
    return population;
}