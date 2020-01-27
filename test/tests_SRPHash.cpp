#include "catch.h"
#include "SRPHash.h"
#include "util.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <fstream>

#include <algorithm>
#include <cmath>



TEST_CASE("SRPHash::getHash", "[SRPHash]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[SRPHash]    getHash" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dim = 10, N = 10;

	SRPHash hash = SRPHash(dim,N);
	// hash.pprint(std::cout);

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

TEST_CASE("SRPHash::behavioral", "[SRPHash]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[SRPHash]    Behavioral" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dim = 10, R = 10000;

	std::vector<double> v(dim,0);
	SRPHash hash = SRPHash(dim,R); 
	// hash.pprint(std::cout); 

	int* hashes = new int[R];
	int* zerohashes = new int[R];
	v[0] = 1; 
	hash.getHash(v,zerohashes);

	for (size_t i = 0; i < dim; i++){
		v[i] = 1; 
		// std::cout<<"|v| = "<<(i+1)*increment*sqrt(dim)<<": "; 
		// for (std::vector<double>::const_iterator i = v.begin(); i != v.end(); ++i)
		    // std::cout << *i << ' ';
		// std::cout<<std::endl; 

		hash.getHash(v,hashes);
		int collisions = 0; 

		for(size_t r = 0; r < R; r++){
			// std::cout<<','<<hashes[r]; 
			if (hashes[r] == zerohashes[r])
				collisions += 1; 
		}
		// std::cout<<std::endl; 

		// sketch.clear(); 
		// sketch.add(hashes);
		double ratio = 1.0/std::sqrt(i+1.0); 
		ratio = std::min(std::max(ratio,-1.0),1.0); 
		std::cout<<1 - std::acos(ratio)* 1.0/PI<<','<<float(collisions) / float(R) <<std::endl; 

	}

	// for (size_t i = 0; i < N; i++)
	// 	std::cout<<sqrt(dim*(i + 1)*increment)<<',';
	// std::cout<<std::endl;


	delete[] zerohashes;
	delete[] hashes;
}
