#pragma once 

#include <limits>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <unordered_map>

#include "Hash.h"

class SRPHash : public Hash {
public:
	SRPHash(size_t dimensions, size_t number_of_hashes, unsigned int s);
	SRPHash(size_t dimensions, size_t number_of_hashes) : SRPHash(dimensions, number_of_hashes, 42) {};
	void getHash(const std::vector<double>& vec, int* hashes);
	void getHash(const std::unordered_map<size_t, double>& vec, int* hashes);
	void pprint(std::ostream &out);
	~SRPHash();

private: 
	size_t _dim; 
	size_t _nhashes;
	double* _C;

	unsigned int seed; 

};




