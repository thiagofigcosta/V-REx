// SLIDE: https://github.com/keroro824/HashingDeepLearning 

#pragma once

#include <climits>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <random>

#include "Bucket.h"
#include "../Slide.hpp"

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