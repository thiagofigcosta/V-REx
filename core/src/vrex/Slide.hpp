#pragma once

#include <map>
#include <vector>
#include <chrono>

class Network;

#include "slide/Node.h"
#include "slide/Network.h"
#include "Utils.hpp"

#define Slide_MAPLEN 325056

using namespace std;

class Slide{
    public:
        // constructors and destructor
        Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, bool useAdamOt,int *RangePow,
            int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, int Stepsize, SlideMode Mode=SlideMode::SAMPLING,
            SlideHashingFunction HashFunc=SlideHashingFunction::DENSIFIED_WTA, bool printDeltas=false);
        Slide(const Slide& orig);
        virtual ~Slide();

        // methods
        void setWeights(map<string, vector<float>> loadedData);
        map<string, vector<float>> getWeights();
        static NodeType* getStdLayerTypes(const int amount_layers);
        vector<float> train(vector<pair<vector<int>, vector<float>>> &train_data,int epochs);
        vector<pair<float,float>> train(vector<pair<vector<int>, vector<float>>> &train_data,vector<pair<vector<int>, vector<float>>> &validation_data,int epochs);
        float evalLoss(vector<pair<vector<int>, vector<float>>> &eval_data);
        pair<int,vector<vector<pair<int,float>>>> evalData(vector<pair<vector<int>, vector<float>>> &test_data);
        void allocAndCastDatasetToSlide(vector<pair<vector<int>, vector<float>>> &data,float **&values, int *&sizes, int **&records, int **&labels, int *&labelsize);
        void deallocSlideDataset(float **values, int *sizes, int **records, int **labels, int *labelsize);

        // variables
        static const float ADAM_OT_BETA1;
        static const float ADAM_OT_BETA2;
        static const float ADAM_OT_EPSILON;
        static const int BUCKET_SIZE;
        static const int TOPK_HASH_TOPK;
        static const int SIMHASH_RATIO;
        static const int WTA_BIN_SIZE;
        static const int TOPK_THRESHOLD_SECONDS;
        static const bool FIFO_INSTEAD_OF_RESERVOIR_SAMPLING;

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
        bool print_deltas;
        bool use_adam;
};