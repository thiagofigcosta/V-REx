// SLIDE: https://github.com/keroro824/HashingDeepLearning 
#pragma once

#include <iostream>
#include <memory>

using namespace std;

#include "../Slide.hpp"

class Bucket
{
private:
	
	#if USE_SMART_POINTERS == 1
		shared_ptr<int[]> arr;
	#else
		int *arr;
	#endif
	int isInit = -1;
	int index = 0;
	int _counts = 0;
	
public:
	Bucket();
	int add(int id);
	int retrieve(int index);
	#if USE_SMART_POINTERS == 1
		shared_ptr<int[]> getAll();
	#else
		int * getAll();
	#endif
	int getTotalCounts();
	int getSize();
	~Bucket();
};


