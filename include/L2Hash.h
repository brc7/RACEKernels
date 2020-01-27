#pragma once 

#include <limits>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <unordered_map>

#include "Hash.h"

class L2Hash : public Hash{
public:
	L2Hash(size_t dimensions, size_t number_of_hashes, double w, unsigned int s);
	L2Hash(size_t dimensions, size_t number_of_hashes, double w) : L2Hash(dimensions,number_of_hashes,w,42) {};
	// int* getHash(const std::vector<double>& vec);
	void getHash(const std::vector<double>& vec, int* hashes);
	// int* getHash(const std::unordered_map<size_t, double>& vec); 
	void getHash(const std::unordered_map<size_t, double>& vec, int* hashes);
	void pprint(std::ostream &out);
	~L2Hash();

private: 
	size_t _dim; 
	size_t _nhashes;
	double _w;
	double* _C;
	double* _b;

	unsigned int seed; 

};




