#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <memory>

using namespace std;

class GarbageCollector{
    private:
        // variables
        static GarbageCollector* singleton;
        #pragma omp threadprivate(singleton)
        vector<int*> int_1_pointers;
        vector<float*> float_1_pointers;
        vector<int**> int_2_pointers;
        vector<float**> float_2_pointers;

    public:
        // constructors and destructor
        GarbageCollector(){}

        // methods
        static GarbageCollector* get(){
            if(singleton==nullptr)
                singleton = new GarbageCollector();
            return singleton;
        }
        static void erase(){
            if(singleton!=nullptr){
                singleton->flush();
                delete singleton;
                singleton=nullptr;
            }
        }
    
        void addInt1d(int* obj);
        void addFloat1d(float* obj);
        void addInt2d(int** obj);
        void addFloat2d(float** obj);
        void flush();
};