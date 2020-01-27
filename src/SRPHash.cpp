#include "SRPHash.h"
#include <iostream>

SRPHash::SRPHash(size_t dimensions, size_t number_of_hashes, unsigned int s)
{
	seed = s; 
	_dim = dimensions; 
	_nhashes = number_of_hashes; 

	_C = new double[_nhashes * _dim]; 
	
	std::mt19937 generator(seed);
	std::normal_distribution<double> gaussian(0,1.0);
	
	// initialize C with a gaussian random variable iid N(0,1)
	for (size_t i = 0; i < (_nhashes*_dim); i++){
		_C[i] = gaussian(generator);
	}
}

SRPHash::~SRPHash(){
	delete[] _C; 
}


void SRPHash::getHash(const std::vector<double>& vec, int* hashes){
	#pragma omp parallel for
	for (size_t k = 0; k < _nhashes; k++){
		double value = 0;
		// inner products between _C[k] and vec
		for (size_t i = 0; i < _dim; i++){
			value += _C[k*_dim + i] * vec[i];
		}
		if (value > 0){
			hashes[k] = 1; 
		} else {
			hashes[k] = 0; 
		}
	}
}


void SRPHash::getHash(const std::unordered_map<size_t, double>& vec, int* hashes){
	#pragma omp parallel for
	for (size_t k = 0; k < _nhashes; k++){
		double value = 0;
		for (const auto& entry : vec){
			size_t i = entry.first; 
			value += _C[k*_dim + i] * entry.second;
		}
		if (value > 0){
			hashes[k] = 1; 
		} else {
			hashes[k] = 0; 
		}
	}
}

void SRPHash::pprint(std::ostream &out){

	out<<"C: ";

	for (size_t k = 0; k < _nhashes; k++){

		out<<'['<<_C[k*_dim]; 
		for (size_t i = 1; i < _dim; i++){
			out<<','<<_C[k*_dim + i]; 
		}
		out<<']'<<std::endl; 
	}

}

