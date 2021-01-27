// SLIDE: https://github.com/keroro824/HashingDeepLearning 

#pragma once

#include <climits>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <random>
#include <memory>
#include <vector>

using namespace std;
class Bucket;

#include "../Slide.hpp"

class LSH {
private:
	#if USE_SMART_POINTERS == 1
		vector<vector<shared_ptr<Bucket>>> _bucket;
	#else
		Bucket ** _bucket;
	#endif
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
	#if USE_SMART_POINTERS == 1
		vector<shared_ptr<int[]>> retrieveRaw(int *indices);
	#else
		int** retrieveRaw(int *indices);
	#endif
	int retrieve(int table, int indices, int bucket);
	void count();
	~LSH();
};
