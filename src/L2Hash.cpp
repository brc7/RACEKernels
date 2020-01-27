#include "L2Hash.h"


#include <iostream>

L2Hash::L2Hash(size_t dimensions, size_t number_of_hashes, double w, unsigned int s)
{
	seed = s; 
	_dim = dimensions; 
	_nhashes = number_of_hashes; 
	_w = w; 

	_b = new double[_nhashes];
	_C = new double[_nhashes * _dim]; 
	
	std::mt19937 generator(seed);
	std::normal_distribution<double> gaussian(0,1.0);
	std::uniform_real_distribution<double> uniform(0.0,_w);
	
	// initialize C with a gaussian random variable iid N(0,1)
	for (size_t i = 0; i < (_nhashes*_dim); i++){
		_C[i] = gaussian(generator);
		// std::cout<<'('<<i<<','<<i/_dim<<','<<i%_dim<<") : "<<_C[i]<<std::endl; 
	}

	// initialize b with a uniform random variable 
	std::cout<<std::endl;
	for (size_t i = 0; i < _nhashes; i++)
		_b[i] = uniform(generator);
}

L2Hash::~L2Hash(){
	delete[] _C; 
	delete[] _b; 
}


void L2Hash::getHash(const std::vector<double>& vec, int* hashes){
	#pragma omp parallel for
	for (size_t k = 0; k < _nhashes; k++){
		double value = 0;
		// inner products between _C[k] and vec
		for (size_t i = 0; i < _dim; i++){
			value += _C[k*_dim + i] * vec[i];
		}
		value += _b[k]; 
		hashes[k] = floor(value / _w); 
	}
}


void L2Hash::getHash(const std::unordered_map<size_t, double>& vec, int* hashes){
	#pragma omp parallel for
	for (size_t k = 0; k < _nhashes; k++){
		double value = 0;
		for (const auto& entry : vec){
			size_t i = entry.first; 
			value += _C[k*_dim + i] * entry.second;
		}
		value += _b[k]; 
		hashes[k] = floor(value / _w); 
	}
}

void L2Hash::pprint(std::ostream &out){

	out<<"b: ["<<_b[0];
	for(size_t k = 1; k < _nhashes; k++)
		out<<','<<_b[k];
	out<<']'<<std::endl;

	out<<"C: ";

	for (size_t k = 0; k < _nhashes; k++){

		out<<'['<<_C[k*_dim]; 
		for (size_t i = 1; i < _dim; i++){
			out<<','<<_C[k*_dim + i]; 
		}
		out<<']'<<std::endl; 
	}

}

