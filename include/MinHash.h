#pragma once 

#include <limits>
#include <vector>

#include "MurmurHash.h"


class MinHash {
private: 
	int _dim; 
	int _numhashes; 

public: 
	MinHash(int dimension, int number_of_hashes); 
	int* getHash(std::vector<int>& vec); 
	void getHash(std::vector<int>& vec, int* hashes); 
	unsigned int internalHash(int input, int seed); 
}; 



