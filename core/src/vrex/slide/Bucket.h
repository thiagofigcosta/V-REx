// SLIDE: https://github.com/keroro824/HashingDeepLearning 
#pragma once

#include <iostream>
#include <memory>

using namespace std;

#include "../Slide.hpp"

class Bucket
{
private:
	
	int* arr;
	int isInit = -1;
	int index = 0;
	int _counts = 0;
	
public:
	Bucket();
	int add(int id);
	int retrieve(int index);
	int* getAll();
	int getTotalCounts();
	int getSize();
	~Bucket();
};


