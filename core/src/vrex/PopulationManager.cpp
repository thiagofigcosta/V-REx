#include "PopulationManager.hpp"
#include "NeuralGenome.hpp"

PopulationManager::PopulationManager(GeneticAlgorithm &galg, SPACE_SEARCH space, function<float(Genome *self)> callback,int startPopulationSize, bool searchHighestFitness, bool useNeuralGenome){
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
    hall_of_fame=NULL;
}

PopulationManager::PopulationManager(const PopulationManager& orig) {
}

PopulationManager::~PopulationManager() {
    ga.release();
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
    for (int g=0;g<gens;){
        for(Genome *individual:population){ // evaluate output
            individual->evaluate();
        }
        ga->fit(population); // calculate fitness
        if (hall_of_fame!=NULL){
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
            sort(population.begin(),population.end());
        }
        // cout<<"Gen: "<<g<<" Size: "<<population.size()<<endl;
    }
}

vector<Genome*> PopulationManager::getPopulation(){
    return population;
}


SPACE_SEARCH PopulationManager::buildSlideNeuralNetworkSpaceSearch(INT_SPACE_SEARCH amount_of_layers,INT_SPACE_SEARCH epoachs,FLOAT_SPACE_SEARCH alpha,
                            INT_SPACE_SEARCH batch_size,INT_SPACE_SEARCH layer_size,INT_SPACE_SEARCH range_pow,INT_SPACE_SEARCH k_values,INT_SPACE_SEARCH l_values,
                            FLOAT_SPACE_SEARCH sparcity,INT_SPACE_SEARCH activation_funcs){
    vector<INT_SPACE_SEARCH> int_dna;
    vector<FLOAT_SPACE_SEARCH> float_dna;
    float_dna.push_back(alpha);
    int_dna.push_back(epoachs);
    int_dna.push_back(batch_size);
    int_dna.push_back(amount_of_layers);
    int max_layer_size=amount_of_layers.second;
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(layer_size);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(range_pow);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(k_values);
    }
    for(int i=0;i<max_layer_size;i++){
        int_dna.push_back(l_values);
    }
    for(int i=0;i<max_layer_size-1;i++){
        float_dna.push_back(sparcity);
    }
    for(int i=0;i<max_layer_size-1;i++){
        int_dna.push_back(activation_funcs);
    }
    return SPACE_SEARCH(int_dna,float_dna);
}