#include "EnchancedGenetic.hpp"


const float EnchancedGenetic::will_of_D_percent=0.05;
const float EnchancedGenetic::recycle_threshold_percent=0.1;

EnchancedGenetic::EnchancedGenetic(int maxChildren, int maxAge, float mutationRate, float sexRate, float recycleRate){
    mutation_rate=mutationRate;
    sex_rate=sexRate;
    recycle_rate=recycleRate;
    max_children=maxChildren;
    max_children=maxChildren;
    max_age=maxAge;
    max_population=0;
}

EnchancedGenetic::EnchancedGenetic(const EnchancedGenetic& orig){
    index_age=orig.index_age;
    index_max_age=orig.index_max_age;
    index_max_children=orig.index_max_children;
    max_population=orig.max_population;
    max_age=orig.max_age;
    max_children=orig.max_children;
    mutation_rate=orig.mutation_rate;
    sex_rate=orig.sex_rate;
    recycle_rate=orig.recycle_rate;
    looking_highest_fitness=orig.looking_highest_fitness;
}

EnchancedGenetic::~EnchancedGenetic(){
}

void EnchancedGenetic::setMaxPopulation(int maxPopulation){
    max_population=maxPopulation;
}

vector<Genome> EnchancedGenetic::select(vector<Genome> &currentGen){
    // roulette wheel
    sort(currentGen.begin(),currentGen.end());
    float min=currentGen[0].getFitness();
    float offset=0;
    float fitness_sum=0;
    if (min<=0){
        offset=abs(min);
    }
    for (Genome &g:currentGen){
        fitness_sum+=g.getFitness()+offset;
    }
    vector<Genome> nxt_gen;
    uniform_real_distribution<float> roulette(0,fitness_sum);
    for (size_t i=0;i<currentGen.size()/2;i++){
        vector<Genome> parents;
        int backup=-1;
        for (int c=0;c<2;c++){
            float sorted=roulette(Utils::RNG);
            float roulette_val=0;
            for(size_t g=0;g<currentGen.size();g++){
                roulette_val+=(currentGen[g].getFitness()+offset);
                if (sorted<=roulette_val){
                    if ( parents.size()<1 || !isRelative(parents[0],currentGen[g])){
                        parents.push_back(currentGen[g]);
                        break;
                    }else if (backup<0){
                        backup=g;
                    }
                }
            }
        }
        vector<Genome> children;
        if (parents.size()==2){
            current_population_size=currentGen.size();
            children=sex(parents[0],parents[1]);
            nxt_gen.insert(nxt_gen.end(),children.begin(),children.end());
        }else{
            nxt_gen.push_back(parents[0]);
            nxt_gen.push_back(currentGen[backup]);
        }
    }
    return nxt_gen;
}

vector<Genome> EnchancedGenetic::fit(vector<Genome> &currentGen){
    int signal=1;
    if (!looking_highest_fitness){
        signal=-1;
    }
    int i=0;
    bool recycled=false;
    while (i<1||recycled){
        for (Genome &g:currentGen){
            g.setFitness(g.getOutput()*signal);
        }
        sort(currentGen.begin(),currentGen.end());
        for (size_t i=0;i<currentGen.size();i++){
            currentGen[i].setFitness(i+1);
        }
        if (i++<1){
            pair<bool,vector<Genome>> out=recycleBadIndividuals(currentGen);
            recycled=out.first;
            currentGen=out.second;
        }else{
            recycled=false;
            size_t cutout_limit=max_population*1.5;
            if(currentGen.size()>cutout_limit){
                currentGen.erase(currentGen.begin(),currentGen.begin()+(currentGen.size()-cutout_limit));
            }
        }
    }
    return currentGen;
}

vector<Genome> EnchancedGenetic::sex(Genome father, Genome mother){
    vector<Genome> family;
    if (Utils::getRandomBetweenZeroAndOne()<sex_rate){
        pair<vector<int>,vector<float>> father_dna=father.getDna();
        pair<vector<int>,vector<float>> mother_dna=mother.getDna();
        float child_rnd=Utils::getRandomBetweenZeroAndOne();
        int childs=child_rnd*father_dna.first[index_max_children]+(1-child_rnd)*mother_dna.first[index_max_children];
        childs=ceil(childs*calcBirthRate(current_population_size));
        for (int c=0;c<childs;c++){
            pair<vector<int>,vector<float>> child=pair<vector<int>,vector<float>>(vector<int>(),vector<float>());
            for(size_t i=0;i<father_dna.first.size();i++){
                if (i != index_age){
                    float gene_share=Utils::getRandomBetweenZeroAndOne();
                    bool heritage_mother=Utils::getRandomBetweenZeroAndOne()>0.5;
                    if (heritage_mother){
                        child.first.push_back((1-gene_share)*father_dna.first[i]+gene_share*mother_dna.first[i]);
                    }else{
                        child.first.push_back(gene_share*father_dna.first[i]+(1-gene_share)*mother_dna.first[i]);
                    }
                }else{
                    child.first.push_back(0); //age
                }
            }
            for(size_t i=0;i<father_dna.second.size();i++){
                float gene_share=Utils::getRandomBetweenZeroAndOne();
                bool heritage_mother=Utils::getRandomBetweenZeroAndOne()>0.5;
                if (heritage_mother){
                    child.second.push_back((1-gene_share)*father_dna.second[i]+gene_share*mother_dna.second[i]);
                }else{
                    child.second.push_back(gene_share*father_dna.second[i]+(1-gene_share)*mother_dna.second[i]);
                }
            }
            family.push_back(Genome(mother,child));
        }
    }
    family.push_back(father);
    family.push_back(mother);
    return family;
}

pair<bool,Genome> EnchancedGenetic::age(Genome &individual, int cur_population_size){
    pair<vector<int>,vector<float>> dna=individual.getDna();
    dna.first[index_age]++;
    individual.setDna(dna);
    if (dna.first[index_age]>dna.first[index_max_age]){
        if (individual.getFitness()<=(1-will_of_D_percent)*cur_population_size){
            return pair<bool,Genome>(false,individual); // dead
        }else{
            individual.resetMtDna();
        }
    }
    return pair<bool,Genome>(true,individual);
}

vector<Genome> EnchancedGenetic::mutate(vector<Genome> &individuals){
    vector<Genome> xmen;
    int cur_pop_size=individuals.size();
    for (Genome &g:individuals){
        pair<bool,Genome> out=age(g,cur_pop_size);
        if (out.first){
            xmen.push_back(mutate(out.second));
        }
    }
    return xmen;
}

unique_ptr<GeneticAlgorithm> EnchancedGenetic::clone(){
    return make_unique<EnchancedGenetic>(*this); 
}

float EnchancedGenetic::randomize(){
    float r=Utils::getRandomBetweenZeroAndOne();
    if (r<=0.3){
        uniform_real_distribution<float> random(0,0.07);
        r=random(Utils::RNG);
    }else if (r<=0.5){
        uniform_real_distribution<float> random(0,0.11);
        r=random(Utils::RNG);
    }else if (r<=0.6){
        uniform_real_distribution<float> random(0.03,0.13);
        r=random(Utils::RNG);
    }else if (r<=0.7){
        uniform_real_distribution<float> random(0.06,0.15);
        r=random(Utils::RNG);
    }else if (r<=0.8){
        uniform_real_distribution<float> random(0.08,0.24);
        r=random(Utils::RNG);
    }else if (r<=0.9){
        uniform_real_distribution<float> random(0.1,0.27);
        r=random(Utils::RNG);
    }else if (r<=0.96){
        uniform_real_distribution<float> random(0.23,0.30);
        r=random(Utils::RNG);
    }else{
        uniform_real_distribution<float> random(0.333,0.666);
        r=random(Utils::RNG);
    }
    if (Utils::getRandomBetweenZeroAndOne()>0.5){
        r=-(1+r);
    }else{
        r=(1+r);
    }
    return r;
}

Genome EnchancedGenetic::mutate(Genome &individual){
    pair<vector<int>,vector<float>> dna=individual.getDna();
    for(size_t i=0;i<dna.first.size();i++){
        if (i != index_age && i!= index_max_age && i != index_max_children) {
            if (Utils::getRandomBetweenZeroAndOne()<mutation_rate){
                dna.first[i]*=randomize();
            }
        }
    }
    for(size_t i=0;i<dna.second.size();i++){
        if (Utils::getRandomBetweenZeroAndOne()<mutation_rate){
            dna.second[i]*=randomize();
        }
    }
    individual.setDna(dna);

    int age=dna.first[index_age];
    individual.checkLimits();

    dna=individual.getDna();
    dna.first[index_age]=age;
    individual.setDna(dna);
    return individual;
}

bool EnchancedGenetic::isRelative(Genome &father, Genome &mother){
    return father.getMtDna()==mother.getMtDna();
}

int EnchancedGenetic::getLifeLeft(Genome &individual){
    return individual.getDna().first[index_max_age]-individual.getDna().first[index_age];
}

pair<bool,vector<Genome>> EnchancedGenetic::recycleBadIndividuals(vector<Genome> &individuals){
    int threshold=(int)individuals.size()*recycle_threshold_percent;
    bool recycled=false;
    sort(individuals.begin(),individuals.end());
    for (Genome &g:individuals){
        if (g.getFitness()<threshold){
            if (Utils::getRandomBetweenZeroAndOne()<recycle_rate){
                int idx_mirror=individuals.size()-(will_of_D_percent*individuals.size()*Utils::getRandomBetweenZeroAndOne())-1; // exploit
                g.setDna(individuals[idx_mirror].getDna());
                g=mutate(g); // explore
                g.evaluate();
                recycled=true;
            }
        }else{
            break;
        }
    }
    return pair<bool,vector<Genome>>(recycled,individuals);
}


float EnchancedGenetic::calcBirthRate(int cur_population_size){
    return ((max_population-cur_population_size)/(max_population/2.22))*abs(randomize());
}

SPACE_SEARCH EnchancedGenetic::enrichSpace(SPACE_SEARCH &space){
    index_age=space.first.size();
    space.first.push_back(pair<int,int>(0,0));
    index_max_age=space.first.size();
    space.first.push_back(pair<int,int>(max_age/2,max_age*abs(randomize())));
    index_max_children=space.first.size();
    space.first.push_back(pair<int,int>(1,max_children*abs(randomize())));
    return space;
}
