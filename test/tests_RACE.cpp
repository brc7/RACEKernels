#include "catch.h"
#include "RACE.h"
#include "MurmurHash.h"
#include "L2Hash.h"


#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <fstream>


TEST_CASE("RACE::add", "[RACE]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RACE]    add" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t R = 10, range = 10;

	// initialize some fake hashes 
	int* hashes = new int[R];
	for(size_t i = 0; i < R; i++){
		hashes[i] = i;
	}

	RACE algorithm = RACE(R,range); 
	algorithm.add(hashes); 
	algorithm.pprint(std::cout); 
	delete[] hashes; 
}

TEST_CASE("RACE::query","[RACE]"){
	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RACE]    query" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t R = 8, range = 8; 
	RACE algorithm = RACE(R, range); 
	size_t N = 500;

	int* hashes = new int[R];

	for(size_t n = 0; n < N; n++){
		for(size_t i = 0; i < R; i++){
			int input = i+n; 
			hashes[i] = MurmurHash (&input, sizeof(int), 0);
		}
		algorithm.add(hashes);
	}

	algorithm.pprint(std::cout);
	for(size_t i = 0; i < R; i++){
		hashes[i] = i;
	}

	double mean = algorithm.query(hashes); 

	std::cout << std::endl << "Mean along diagonal "; 
	std::cout << mean << std::endl; 


	delete[] hashes; 
}


TEST_CASE("RACE::clear","[RACE]"){
	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RACE]    clear" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t R = 8, range = 8; 
	RACE algorithm = RACE(R, range); 
	size_t N = 500;

	int* hashes = new int[R];

	for(size_t n = 0; n < N; n++){
		for(size_t i = 0; i < R; i++){
			int input = i+n; 
			hashes[i] = MurmurHash (&input, sizeof(int), 0);
		}
		algorithm.add(hashes);
	}

	algorithm.pprint(std::cout);
	algorithm.clear(); 
	algorithm.pprint(std::cout);

	delete[] hashes; 
}





TEST_CASE("RACE::serialize/deserialize","[RACE]"){
	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RACE]    serialize" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t R = 4, range = 12; 
	RACE algorithm = RACE(R, range); 
	int N = 500;

	int* hashes = new int[R];

	for(size_t n = 0; n < N; n++){
		for(size_t i = 0; i < R; i++){
			hashes[i] = i*n;
		}
		algorithm.add(hashes);
	}

	// now save to an in-memory stream using serialize 
	algorithm.pprint(std::cout);

	std::cout<<std::endl<<"After serialization and deserialization:"<<std::endl<<std::endl; 	
	std::stringstream s; 
	algorithm.serialize(s); 
	s.seekg(0, std::ios::beg);
	algorithm.deserialize(s); 

	algorithm.pprint(std::cout); 
	delete[] hashes; 
}



TEST_CASE("RACE::behavioral", "[RACE]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[RACE]    Behavioral" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dim = 10, R = 5000, range = 10; 
	size_t NSim = 2000;

	double simulation_range = 500;
	double increment = simulation_range / NSim;

	int w = 100;

	std::vector<double> v(dim,0);

	RACE sketch = RACE(R, range);

	L2Hash hash = L2Hash(dim,R,w);

	int* hashes = new int[R];
	int* zerohashes = new int[R];
	hash.getHash(v,zerohashes);

	for (size_t i = 0; i < NSim; i++){
		std::fill(v.begin(), v.end(), (i + 1)*increment);
		for (size_t j = 0; j < v.size(); j++){
			if (j % 2 == 0){
				v[j] = -1*v[j]; 
			}
		}
		hash.getHash(v,hashes);
		sketch.clear();
		sketch.add(hashes);

		std::cout<<(i+1)*increment*sqrt(dim)<<','<<sketch.query(zerohashes)<<std::endl;

	}

	delete[] zerohashes;
	delete[] hashes;
}





