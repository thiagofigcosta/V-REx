#include "GarbageCollector.hpp"

GarbageCollector* GarbageCollector::singleton=nullptr;

void GarbageCollector::addInt1d(int* obj){
    int_1_pointers.push_back(obj);
}

void GarbageCollector::addFloat1d(float* obj){
    float_1_pointers.push_back(obj);
}

void GarbageCollector::addInt2d(int** obj){
    int_2_pointers.push_back(obj);
}

void GarbageCollector::addFloat2d(float** obj){
    float_2_pointers.push_back(obj);
}

void GarbageCollector::flush(){
    while(int_1_pointers.size()>0){
        delete[] int_1_pointers.back();
        int_1_pointers.pop_back();
    }
    while(float_1_pointers.size()>0){
        delete[] float_1_pointers.back();
        float_1_pointers.pop_back();
    }
    while(int_2_pointers.size()>0){
        delete[] int_2_pointers.back();
        int_2_pointers.pop_back();
    }
    while(float_2_pointers.size()>0){
        delete[] float_2_pointers.back();
        float_2_pointers.pop_back();
    }
}