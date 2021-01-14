// SLIDE: https://github.com/keroro824/HashingDeepLearning 

#include "Network.h"
#include "Config.h"

using namespace std;

Network::Network(int *sizesOfLayers, NodeType *layersTypes, int noOfLayers, int batchSize, float lr, int inputdim,  int* K, int* L, int* RangePow, float* Sparsity,SlideMode Mode,SlideHashingFunction hashFunc, bool useAdamOt) {
    _numberOfLayers = noOfLayers;
    _hiddenlayers = new Layer *[noOfLayers];
    _sizesOfLayers = sizesOfLayers;
    _layersTypes = layersTypes;
    _learningRate = lr;
    _currentBatchSize = batchSize;
    _Sparsity = Sparsity;
    _inputDim=inputdim;
	_K=K;
	_L=L;
	_RangePow=RangePow;
    hash_func=hashFunc;
    mode=Mode;
    use_adam=useAdamOt;
    for (int i = 0; i < noOfLayers; i++) {
        #pragma GCC diagnostic push 
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        if (i != 0) {
            _hiddenlayers[i] = new Layer(sizesOfLayers[i], sizesOfLayers[i - 1], i, _layersTypes[i], _currentBatchSize,  K[i], L[i], RangePow[i], Sparsity[i],mode,hash_func, nullptr, nullptr, nullptr, nullptr);
        } else {
            _hiddenlayers[i] = new Layer(sizesOfLayers[i], inputdim, i, _layersTypes[i], _currentBatchSize, K[i], L[i], RangePow[i], Sparsity[i],mode,hash_func, nullptr, nullptr, nullptr, nullptr);
        }
        #pragma GCC diagnostic pop
    }
}

void Network::setWeights(map<string, vector<float>> loadedData){
    for (int i = 0; i < _numberOfLayers; i++) {
        float* weight, *bias, *adamAvgMom, *adamAvgVel;
        if (i != 0) {
            weight = &loadedData["w_layer_"+to_string(i)][0];
            bias = &loadedData["b_layer_"+to_string(i)][0];
            adamAvgMom = &loadedData["am_layer_"+to_string(i)][0];
            adamAvgVel = &loadedData["av_layer_"+to_string(i)][0];
            delete _hiddenlayers[i];
            _hiddenlayers[i] = new Layer(_sizesOfLayers[i], _sizesOfLayers[i - 1], i, _layersTypes[i], _currentBatchSize,  _K[i], _L[i], _RangePow[i], _Sparsity[i],mode,hash_func, weight, bias, adamAvgMom, adamAvgVel);
        } else {
            weight = &loadedData["w_layer_"+to_string(i)][0];
            bias = &loadedData["b_layer_"+to_string(i)][0];
            adamAvgMom = &loadedData["am_layer_"+to_string(i)][0];
            adamAvgVel = &loadedData["av_layer_"+to_string(i)][0];
            delete _hiddenlayers[i];
            _hiddenlayers[i] = new Layer(_sizesOfLayers[i], _inputDim, i, _layersTypes[i], _currentBatchSize, _K[i], _L[i], _RangePow[i], _Sparsity[i],mode,hash_func, weight, bias, adamAvgMom, adamAvgVel);
        }
    }
}


Layer *Network::getLayer(int LayerID) {
    if (LayerID < _numberOfLayers)
        return _hiddenlayers[LayerID];
    else {
        cout << "LayerID out of bounds" << endl;
        //TODO:Handle
        return NULL;
    }
}


pair<int,vector<vector<pair<int,float>>>> Network::predictClass(int **inputIndices, float **inputValues, int *length, int **labels, int *labelsize) {
    int correctPred = 0;
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wsizeof-pointer-div"
    int last_layer_size=labelsize[(sizeof(labelsize)/sizeof(*labelsize))-1];
    #pragma GCC diagnostic pop 
    vector<vector<pair<int,float>>> predicted_classes(_currentBatchSize, vector<pair<int,float>>(last_layer_size, pair<int,float>(0,0.0)));
#pragma omp parallel for reduction(+:correctPred)
    for (int i = 0; i < _currentBatchSize; i++) {
        int **activenodesperlayer = new int *[_numberOfLayers + 1]();
        float **activeValuesperlayer = new float *[_numberOfLayers + 1]();
        int *sizes = new int[_numberOfLayers + 1]();

        activenodesperlayer[0] = inputIndices[i];
        activeValuesperlayer[0] = inputValues[i];
        sizes[0] = length[i];

        //inference
        for (int j = 0; j < _numberOfLayers; j++) {
            _hiddenlayers[j]->queryActiveNodeandComputeActivations(activenodesperlayer, activeValuesperlayer, sizes, j, i, labels[i], 0,
                    _Sparsity[_numberOfLayers+j], -1);
        }

        //compute softmax
        int noOfClasses = sizes[_numberOfLayers];
        float max_act = numeric_limits<int>::min();
        int predicted_class_pos = -1;
        for (int k = 0; k < noOfClasses; k++) {
            float cur_act = _hiddenlayers[_numberOfLayers - 1]->getNodebyID(activenodesperlayer[_numberOfLayers][k])->getLastActivation(i);
            if (max_act < cur_act) {
                max_act = cur_act;
                predicted_class_pos = activenodesperlayer[_numberOfLayers][k];
            }
            predicted_classes[i][k].second=cur_act;
        }
        predicted_classes[i][predicted_class_pos].first=labels[i][predicted_class_pos];
        for (int c=0;c<labelsize[i];c++){
            if (labels[i][c]==predicted_classes[i][c].first){
                if (c+1==labelsize[i]){
                    correctPred++;
                }
            }else{
                break;
            }
        }
        delete[] sizes; 
        for (int j = 1; j < _numberOfLayers + 1; j++) {
            delete[] activenodesperlayer[j];
            delete[] activeValuesperlayer[j];
        }
        delete[] activenodesperlayer;
        delete[] activeValuesperlayer;
    }
    return pair<int,vector<vector<pair<int,float>>>>(correctPred,predicted_classes);
}


float Network::ProcessInput(int **inputIndices, float **inputValues, int *lengths, int **labels, int *labelsize, int iter, bool rehash, bool rebuild) {

    // float logloss = 0.0; // not used
    int* avg_retrieval = new int[_numberOfLayers]();

    for (int j = 0; j < _numberOfLayers; j++)
        avg_retrieval[j] = 0;


    if(iter%6946==6945 ){
        //_learningRate *= 0.5;
        _hiddenlayers[1]->updateRandomNodes();
    }
    float tmplr = _learningRate;
    if (use_adam) {
        tmplr = _learningRate * sqrt((1 - pow(Slide::ADAM_OT_BETA2, iter + 1))) /
                (1 - pow(Slide::ADAM_OT_BETA1, iter + 1));
    }else{
//        tmplr *= pow(0.9, iter/10.0);
    }

    int*** activeNodesPerBatch = new int**[_currentBatchSize];
    float*** activeValuesPerBatch = new float**[_currentBatchSize];
    int** sizesPerBatch = new int*[_currentBatchSize];
    float* grads = new float[_currentBatchSize];
#pragma omp parallel for
    for (int i = 0; i < _currentBatchSize; i++) {
        int **activenodesperlayer = new int *[_numberOfLayers + 1]();
        float **activeValuesperlayer = new float *[_numberOfLayers + 1]();
        int *sizes = new int[_numberOfLayers + 1]();

        activeNodesPerBatch[i] = activenodesperlayer;
        activeValuesPerBatch[i] = activeValuesperlayer;
        sizesPerBatch[i] = sizes;

        activenodesperlayer[0] = inputIndices[i];  // inputs parsed from training data file
        activeValuesperlayer[0] = inputValues[i];
        sizes[0] = lengths[i];
        int in;
        // forward propagation
        for (int j = 0; j < _numberOfLayers; j++) {
            in = _hiddenlayers[j]->queryActiveNodeandComputeActivations(activenodesperlayer, activeValuesperlayer, sizes, j, i, labels[i], labelsize[i],
                    _Sparsity[j], iter*_currentBatchSize+i);
            avg_retrieval[j] += in;
        }

        //Now backpropagate.
        // layers
        grads[i]=0;
        for (int j = _numberOfLayers - 1; j >= 0; j--) {
            Layer* layer = _hiddenlayers[j];
            Layer* prev_layer = _hiddenlayers[j - 1];
            // nodes
            for (int k = 0; k < sizesPerBatch[i][j + 1]; k++) {
                Node* node = layer->getNodebyID(activeNodesPerBatch[i][j + 1][k]);
                if (j == _numberOfLayers - 1) {
                    //TODO: Compute Extra stats: labels[i];
                    node->ComputeExtaStatsForSoftMax(layer->getNomalizationConstant(i), i, labels[i], labelsize[i]);
                }
                if (j != 0) {
                    grads[i]+=node->backPropagate(prev_layer->getAllNodes(), activeNodesPerBatch[i][j], sizesPerBatch[i][j], tmplr, i);
                } else {
                    grads[i]+=node->backPropagateFirstLayer(inputIndices[i], inputValues[i], lengths[i], tmplr, i);
                }
            }
        }
    }
    for (int i = 0; i < _currentBatchSize; i++) {
        //Free memory to avoid leaks
        delete[] sizesPerBatch[i];
        for (int j = 1; j < _numberOfLayers + 1; j++) {
            delete[] activeNodesPerBatch[i][j];
            delete[] activeValuesPerBatch[i][j];
        }
        delete[] activeNodesPerBatch[i];
        delete[] activeValuesPerBatch[i];
    }

    delete[] activeNodesPerBatch;
    delete[] activeValuesPerBatch;
    delete[] sizesPerBatch;

    bool tmpRehash;
    bool tmpRebuild;

    for (int l=0; l<_numberOfLayers ;l++) {
        if(rehash & (_Sparsity[l]<1)){
            tmpRehash=true;
        }else{
            tmpRehash=false;
        }
        if(rebuild & (_Sparsity[l]<1)){
            tmpRebuild=true;
        }else{
            tmpRebuild=false;
        }
        if (tmpRehash) {
            _hiddenlayers[l]->_hashTables->clear();
        }
        if (tmpRebuild){
            _hiddenlayers[l]->updateTable();
        }
        int ratio = 1;
#pragma omp parallel for
        for (size_t m = 0; m < _hiddenlayers[l]->_noOfNodes; m++)
        {
            Node *tmp = _hiddenlayers[l]->getNodebyID(m);
            int dim = tmp->_dim;
            float* local_weights = new float[dim];
            std::copy(tmp->_weights, tmp->_weights + dim, local_weights);

            if(use_adam){
                for (int d=0; d < dim;d++){
                    float _t = tmp->_t[d];
                    float Mom = tmp->_adamAvgMom[d];
                    float Vel = tmp->_adamAvgVel[d];
                    Mom = Slide::ADAM_OT_BETA1 * Mom + (1 - Slide::ADAM_OT_BETA1) * _t;
                    Vel = Slide::ADAM_OT_BETA2 * Vel + (1 - Slide::ADAM_OT_BETA2) * _t * _t;
                    local_weights[d] += ratio * tmplr * Mom / (sqrt(Vel) + Slide::ADAM_OT_EPSILON);
                    tmp->_adamAvgMom[d] = Mom;
                    tmp->_adamAvgVel[d] = Vel;
                    tmp->_t[d] = 0;
                }

                tmp->_adamAvgMombias = Slide::ADAM_OT_BETA1 * tmp->_adamAvgMombias + (1 - Slide::ADAM_OT_BETA1) * tmp->_tbias;
                tmp->_adamAvgVelbias = Slide::ADAM_OT_BETA2 * tmp->_adamAvgVelbias + (1 - Slide::ADAM_OT_BETA2) * tmp->_tbias * tmp->_tbias;
                tmp->_bias += ratio*tmplr * tmp->_adamAvgMombias / (sqrt(tmp->_adamAvgVelbias) + Slide::ADAM_OT_EPSILON);
                tmp->_tbias = 0;
                std::copy(local_weights, local_weights + dim, tmp->_weights);
            }
            else
            {
                std::copy(tmp->_mirrorWeights, tmp->_mirrorWeights+(tmp->_dim) , tmp->_weights);
                tmp->_bias = tmp->_mirrorbias;
            }
            if (tmpRehash) {
                int *hashes;
                if(hash_func==SlideHashingFunction::WTA) {
                    hashes = _hiddenlayers[l]->_wtaHasher->getHash(local_weights);
                }else if (hash_func==SlideHashingFunction::DENSIFIED_WTA){
                    hashes = _hiddenlayers[l]->_dwtaHasher->getHashEasy(local_weights, dim, Slide::TOPK_HASH_TOPK);
                }else if (hash_func==SlideHashingFunction::TOPK_MIN_HASH){
                    hashes = _hiddenlayers[l]->_MinHasher->getHashEasy(_hiddenlayers[l]->_binids, local_weights, dim, Slide::TOPK_HASH_TOPK);
                }else if (hash_func==SlideHashingFunction::SIMHASH){
                    hashes = _hiddenlayers[l]->_srp->getHash(local_weights, dim);
                }

                int *hashIndices = _hiddenlayers[l]->_hashTables->hashesToIndex(hashes);
                int * bucketIndices = _hiddenlayers[l]->_hashTables->add(hashIndices, m+1);
                #pragma GCC diagnostic push 
                #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
                delete[] hashes;
                #pragma GCC diagnostic pop 
                delete[] hashIndices;
                delete[] bucketIndices;
            }


            delete[] local_weights;
        }
    }

    float total_grads=0;
    for (size_t i=0;i<(size_t)_currentBatchSize;i++){
        total_grads+=grads[i];
    }

    return total_grads/_currentBatchSize;
}

float Network::evalInput(int** inputIndices, float** inputValues, int* lengths, int ** labels, int *labelsize){
    int*** activeNodesPerBatch = new int**[_currentBatchSize];
    int** sizesPerBatch = new int*[_currentBatchSize];
    float* grads = new float[_currentBatchSize];
#pragma omp parallel for
    for (int i = 0; i < _currentBatchSize; i++) {
        int **activenodesperlayer = new int *[_numberOfLayers + 1]();
        float **activeValuesperlayer = new float *[_numberOfLayers + 1]();
        int *sizes = new int[_numberOfLayers + 1]();

        activeNodesPerBatch[i] = activenodesperlayer;
        sizesPerBatch[i] = sizes;

        activenodesperlayer[0] = inputIndices[i];  // inputs parsed from training data file
        activeValuesperlayer[0] = inputValues[i];
        sizes[0] = lengths[i];
        for (int j = 0; j < _numberOfLayers; j++) {
            _hiddenlayers[j]->queryActiveNodeandComputeActivations(activenodesperlayer, activeValuesperlayer, sizes, j, i, labels[i], 0,
                    _Sparsity[j], -1);
        }

        //Now backpropagate.
        // layers
        grads[i]=0;
        for (int j = _numberOfLayers - 1; j >= 0; j--) {
            Layer* layer = _hiddenlayers[j];
            // nodes
            for (int k = 0; k < sizesPerBatch[i][j + 1]; k++) {
                Node* node = layer->getNodebyID(activeNodesPerBatch[i][j + 1][k]);
                if (j == _numberOfLayers - 1) {
                    //TODO: Compute Extra stats: labels[i];
                    node->ComputeExtaStatsForSoftMax(layer->getNomalizationConstant(i), i, labels[i], labelsize[i]);
                }
                grads[i]+=node->calcBackPropagateGrad(sizesPerBatch[i][j], i);
            }
        }
    }
    for (int i = 0; i < _currentBatchSize; i++) {
        //Free memory to avoid leaks
        delete[] sizesPerBatch[i];
        for (int j = 1; j < _numberOfLayers + 1; j++) {
            delete[] activeNodesPerBatch[i][j];
        }
        delete[] activeNodesPerBatch[i];
    }
    delete[] activeNodesPerBatch;
    delete[] sizesPerBatch;
    float total_grads=0;
    for (size_t i=0;i<(size_t)_currentBatchSize;i++){
        total_grads+=grads[i];
    }
    return total_grads/_currentBatchSize;
}

map<string, vector<float>> Network::mapfyWeights()
{
    map<string, vector<float>> weights;
    for (int i=0; i< _numberOfLayers; i++){
        map<string, vector<float>> layerWeights = _hiddenlayers[i]->mapfyWeights();
        weights.insert(layerWeights.begin(), layerWeights.end());
    }
    return weights;
}


Network::~Network() {
    delete[] _sizesOfLayers;
    // TODO: causing exception: double free or corruption (fasttop)
    // for (int i=0; i< _numberOfLayers; i++){
    //     delete _hiddenlayers[i];
    // }
    delete[] _hiddenlayers;
    delete[] _layersTypes;
}
