// SLIDE: https://github.com/keroro824/HashingDeepLearning 

#include "Bucket.h"
#include "../Slide.hpp"

Bucket::Bucket()
{
    isInit = -1;
    arr = new int[Slide::BUCKET_SIZE]();
}


Bucket::~Bucket()
{
    delete[] arr;
}


int Bucket::getTotalCounts()
{
    return _counts;
}


int Bucket::getSize()
{
    return _counts;
}


int Bucket::add(int id) {

    //FIFO
    if (Slide::FIFO_INSTEAD_OF_RESERVOIR_SAMPLING) {
        isInit += 1;
        int index = _counts & (Slide::BUCKET_SIZE - 1);
        _counts++;
        arr[index] = id;
        return index;
    }
    //Reservoir Sampling
    else {
        _counts++;
        if (index == Slide::BUCKET_SIZE) {
            int randnum = rand() % (_counts) + 1;
            if (randnum == 2) {
                int randind = rand() % Slide::BUCKET_SIZE;
                arr[randind] = id;
                return randind;
            } else {
                return -1;
            }
        } else {
            arr[index] = id;
            int returnIndex = index;
            index++;
            return returnIndex;
        }
    }
}


int Bucket::retrieve(int indice)
{
    if (indice >= Slide::BUCKET_SIZE)
        return -1;
    return arr[indice];
}


int * Bucket::getAll()
{
    if (isInit == -1)
        return NULL;
    if(_counts<Slide::BUCKET_SIZE){
        arr[_counts]=-1;
    }
    return arr;
}
