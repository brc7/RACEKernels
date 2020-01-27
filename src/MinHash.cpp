#include "MinHash.h"

MinHash::MinHash(int dimension, int number_of_hashes){
    _dim = dimension; 
    _numhashes = number_of_hashes;
}

int* MinHash::getHash(std::vector<int>& vec){
    int *hashes = new int[_numhashes];
    unsigned int hashed_value;
    unsigned int minhashed_value;
    // #pragma omp parallel for 
    for (int k=0; k < _numhashes; k++) {

        minhashed_value = std::numeric_limits<unsigned int>::max(); 
        hashes[k] = 0; 
        
        for (int i = 0; i < vec.size(); i++){
            // hashed_value = this->internalHash(vec[i], k); 
            hashed_value = MurmurHash(&(vec[i]), sizeof(int), k); 
            if (hashed_value < minhashed_value){
                minhashed_value = hashed_value; 
                hashes[k] = vec[i]; 
            }
        }
    }
    return hashes; 
}

void MinHash::getHash(std::vector<int>& vec, int* hashes){
    // hashes had better be pre-allocated to _numhashes!! 
    // I do this because this is faster in a loop 

    #pragma omp parallel for 
    for (int k=0; k < _numhashes; k++) {

        unsigned int hashed_value;
        unsigned int minhashed_value;

        minhashed_value = std::numeric_limits<unsigned int>::max(); 
        hashes[k] = 0; 
        
        for (int i = 0; i < vec.size(); i++){
            // hashed_value = this->internalHash(vec[i], k); 
            hashed_value = MurmurHash(&(vec[i]), sizeof(int), k); 
            if (hashed_value < minhashed_value){
                minhashed_value = hashed_value; 
                hashes[k] = vec[i]; 
            }
        }
    }
    return; 
}


unsigned int MinHash::internalHash(int input, int seed)
{
    return MurmurHash(&input, sizeof(int), seed);
}
