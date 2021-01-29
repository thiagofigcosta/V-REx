#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <memory>

#define Slide_MAPLEN 325056

class Bucket;
class Network;
enum class NodeType;
enum class SlideMode { TOPK_THRESHOLD=1, SAMPLING=4, UNKNOWN_MODE1=2, UNKNOWN_MODE2=3 }; // TODO find out mode names
enum class SlideHashingFunction { WTA=1, DENSIFIED_WTA=2, TOPK_MIN_HASH=3, SIMHASH=4 };
enum class SlideLabelEncoding { INT_CLASS, NEURON_BY_NEURON, NEURON_BY_N_LOG_LOSS };

using namespace std;

class Slide{
    public:
        // constructors and destructor
        Slide(int numLayer, int *sizesOfLayers, NodeType* layerTypes, int InputDim, float Lr, int Batchsize, bool useAdamOt, 
            SlideLabelEncoding labelType,int *RangePow, int *KValues,int *LValues,float *Sparsity, int Rehash, int Rebuild, 
            SlideMode Mode=SlideMode::SAMPLING, SlideHashingFunction HashFunc=SlideHashingFunction::DENSIFIED_WTA, bool printDeltas=false);
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
        static const bool MEAN_ERROR_INSTEAD_OF_GRADS_SUM=true;
        #pragma omp threadprivate(MEAN_ERROR_INSTEAD_OF_GRADS_SUM)
        static constexpr float ADAM_OT_BETA1=0.9;
        #pragma omp threadprivate(ADAM_OT_BETA1)
        static constexpr float ADAM_OT_BETA2=0.999;
        #pragma omp threadprivate(ADAM_OT_BETA2)
        static constexpr float ADAM_OT_EPSILON=0.00000001;
        #pragma omp threadprivate(ADAM_OT_EPSILON)
        static const int BUCKET_SIZE=128;
        #pragma omp threadprivate(BUCKET_SIZE)
        static const int TOPK_HASH_TOPK=30;
        #pragma omp threadprivate(TOPK_HASH_TOPK)
        static const int SIMHASH_RATIO=3;
        #pragma omp threadprivate(SIMHASH_RATIO)
        static const int WTA_BIN_SIZE=8; // binsize is the number of times the range is larger than the total number of hashes we need.
        #pragma omp threadprivate(WTA_BIN_SIZE)
        static const int TOPK_THRESHOLD_SECONDS=2;
        #pragma omp threadprivate(TOPK_THRESHOLD_SECONDS)
        static const bool FIFO_INSTEAD_OF_RESERVOIR_SAMPLING=true;
        #pragma omp threadprivate(FIFO_INSTEAD_OF_RESERVOIR_SAMPLING)
        static constexpr float SOFTMAX_LINEAR_CONSTANT=0.0000001;
        #pragma omp threadprivate(SOFTMAX_LINEAR_CONSTANT)
        static constexpr float RAND_WEIGHT_START=0;
        #pragma omp threadprivate(RAND_WEIGHT_START)
        static constexpr float RAND_WEIGHT_END= 0.01;
        #pragma omp threadprivate(RAND_WEIGHT_END)

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
        int *layer_sizes;
        int amount_layers;
        SlideMode mode;
        SlideHashingFunction hash_function;
        bool print_deltas;
        bool use_adam;
        SlideLabelEncoding label_type;
};