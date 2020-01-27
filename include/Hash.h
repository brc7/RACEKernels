#pragma once 

#include <limits>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <unordered_map>

// hash interface 

class Hash
{
public:
    Hash(){}
    virtual ~Hash(){}
	virtual void getHash(const std::vector<double>& vec, int* hashes) = 0; 
	virtual void getHash(const std::unordered_map<size_t, double>& vec, int* hashes) = 0; 
	virtual void pprint(std::ostream &out) = 0;
};

