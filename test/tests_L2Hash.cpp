#include "catch.h"
#include "L2Hash.h"

#include "RACE.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <fstream>

#include <algorithm>
#include <cmath>



TEST_CASE("L2Hash::getHash", "[L2Hash]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[L2Hash]    getHash" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dim = 10, N = 10;
	double w = 0.01;

	L2Hash hash = L2Hash(dim,N,w);
	hash.pprint(std::cout);

	std::vector<double> v(dim,5);
	int* hashes = new int[N]; 
	hash.getHash(v, hashes); 
	
	std::cout<<"Hashes: ["<<hashes[0]; 
	for (size_t i = 1; i < N; i++)
		std::cout<<','<<hashes[i]; 
	std::cout<<']'<<std::endl; 

	std::unordered_map<size_t, double> map; 
	for(size_t i = 0; i < dim; ++i){
		map[i] = 5.0; 
	}
	int* maphashes = new int[N]; 
	hash.getHash(map, maphashes); 
	std::cout<<"MapHashes: ["<<maphashes[0]; 
	for (size_t i = 1; i < N; i++)
		std::cout<<','<<maphashes[i]; 
	std::cout<<']'<<std::endl; 


	// delete[] hashes; 
	// remember to add + range / 2 to the functions!
	// because -index % range = index % range and that's baaaaad
}

TEST_CASE("L2Hash::behavioral", "[L2Hash]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[L2Hash]    Behavioral" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dim = 10, R = 100, NSim = 2000;
	// size_t offset = 10e4; 

	double simulation_range = 500;
	double increment = simulation_range / NSim;

	int w = 100;

	std::vector<double> v(dim,0);

	// RACE sketch = RACE(N, 5000);

	L2Hash hash = L2Hash(dim,R,w); 

	hash.pprint(std::cout); 

	int* hashes = new int[R];
	int* zerohashes = new int[R];
	hash.getHash(v,zerohashes);

	for (size_t i = 0; i < NSim; i++){
		std::fill(v.begin(), v.end(), (i + 1)*increment);
		// std::cout<<"|v| = "<<(i+1)*increment*sqrt(dim)<<": "; 
		// for (std::vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
		    // std::cout << *i << ' ';
		// std::cout<<std::endl; 

		hash.getHash(v,hashes);
		int collisions = 0; 

		for(size_t r = 0; r < R; r++){
			std::cout<<','<<hashes[r]; 
			if (hashes[r] == zerohashes[r])
				collisions += 1; 
		}
		std::cout<<std::endl; 

		// sketch.clear(); 
		// sketch.add(hashes);
		
		std::cout<<(i+1)*increment*sqrt(dim)<<','<<float(collisions) / float(R) <<std::endl; 

	}

	// for (size_t i = 0; i < N; i++)
	// 	std::cout<<sqrt(dim*(i + 1)*increment)<<',';
	// std::cout<<std::endl;


	delete[] zerohashes;
	delete[] hashes;
}
