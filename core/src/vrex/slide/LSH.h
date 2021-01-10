// SLIDE: https://github.com/keroro824/HashingDeepLearning 

#pragma once
#include "Bucket.h"
#include <random>

enum class SlideMode { TOPK_THRESHOLD=1, SAMPLING=4, UNKNOWN_MODE1=2, UNKNOWN_MODE2=3 }; // TODO find out mode names
enum class SlideHashingFunction { WTA=1, DENSIFIED_WTA=2, TOPK_MIN_HASH=3, SIMHASH=4 };

class LSH {
private:
	Bucket ** _bucket;
	int _K;
	int _L;
	int _RangePow;
	int *rand1;
	SlideHashingFunction hash_func;


public:
	LSH(int K, int L, int RangePow,SlideHashingFunction hashFunc);
	void clear();
	int* add(int *indices, int id);
	int add(int indices, int tableId, int id);
	int * hashesToIndex(int * hashes);
	int** retrieveRaw(int *indices);
	int retrieve(int table, int indices, int bucket);
	void count();
	~LSH();
};
