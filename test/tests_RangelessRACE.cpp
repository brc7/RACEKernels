#include "catch.h"
#include "RangelessRACE.h"
#include "MurmurHash.h"
#include "L2Hash.h"


#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <fstream>


TEST_CASE("RangelessRACE::serialize and RangelessRACE::deserialize", "[RangelessRACE]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RangelessRACE]    serialize/deserialize" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 


	size_t R = 4; 

	RangelessRACE algorithm = RangelessRACE(R); 
	int N = 500;
	int* hashes = new int[R];
	for(size_t n = 0; n < N; n++){
		for(size_t i = 0; i < R; i++){
			int input = i+n; 
			hashes[i] = MurmurHash (&input, sizeof(int), 0);
			hashes[i] = hashes[i]%5;
			if (i%2 == 0){
				hashes[i] = -hashes[i]; 
			}
		}
		algorithm.add(hashes);
	}

	// now save to an in-memory stream using serialize 
	algorithm.pprint(std::cout);

	std::cout<<std::endl<<"After serialization and deserialization:"<<std::endl<<std::endl; 	
	std::stringstream s; 
	algorithm.serialize(s); 
	s.seekg(0, std::ios::beg);
	RangelessRACE algorithm2 = RangelessRACE(0); 
	algorithm2.deserialize(s); 
	algorithm2.pprint(std::cout); 
	delete[] hashes; 
}
