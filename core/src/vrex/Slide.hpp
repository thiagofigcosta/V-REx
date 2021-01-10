#pragma once

#include <map>
#include <vector>

#include "slide/Node.h"
#include "slide/Network.h"
#include "Utils.hpp"

using namespace std;

class Slide{
    public:
        // constructors and destructor
        Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, int *RangePow,
            int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, int Stepsize, SlideMode Mode=SlideMode::SAMPLING,
            SlideHashingFunction HashFunc=SlideHashingFunction::DENSIFIED_WTA);
        Slide(const Slide& orig);
        virtual ~Slide();

        // methods
        void setWeights(map<string, vector<float>> loadedData);
        map<string, vector<float>> getWeights();
        static NodeType* getStdLayerTypes(const int amount_layers);
        vector<float> train(vector<pair<int, vector<float>>> train_data,int epochs);
        vector<pair<float,float>> train(vector<pair<int, vector<float>>> train_data,vector<pair<int, vector<float>>> validation_data,int epochs);

    private:
        // variables
        Network *slide_network;
        NodeType* layer_types;
        int *range_pow;
        int *K;
        int *L;
        float *sparsity;
        int batch_size;
        int rehash;
        int rebuild;
        int input_dim;
        float learning_rate;
        int epochs;
        int step_size;
        int *layer_sizes;
        int amount_layers;
        SlideMode mode;
        SlideHashingFunction hash_function;

};