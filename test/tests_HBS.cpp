#include "catch.h"
#include "HBS.h"
#include "MurmurHash.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> 
#include <fstream>


TEST_CASE("HBS::add", "[HBS]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[HBS]    add" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 


	size_t num_tables = 5;
	int* hashes = new int[num_tables];

	HBS sketch = HBS(num_tables);
	// initialize some fake hashes
	for(size_t DataID = 1; DataID < 100; DataID++){
		for(size_t i = 0; i < num_tables; i++){
			hashes[i] = DataID % (i+4);
		}
		sketch.add(hashes,DataID);
	}

	sketch.pprint(std::cout,2); 
	delete[] hashes; 
}


// std::vector<std::pair<size_t, double> >

TEST_CASE("HBS::samples", "[HBS]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[HBS]    samples" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t num_tables = 3;
	int* hashes = new int[num_tables];

	HBS sketch = HBS(num_tables);
	// initialize some fake hashes
	size_t dataset_size = 100; 

	for(size_t DataID = 1; DataID < dataset_size; DataID++){
		for(size_t i = 0; i < num_tables; i++){
			hashes[i] = DataID*DataID*DataID % (12*(i+1));
		}
		sketch.add(hashes,DataID);
	}

	sketch.pprint(std::cout,2);

	std::vector<std::pair<size_t, double> > samples = sketch.samples(40,dataset_size);

	std::cout<<samples.size()<<std::endl; 

	for(size_t i = 0; i < samples.size(); i++){
		if (i % (100/num_tables) == 0)
			std::cout<<"-----------"<<std::endl; 
		std::cout<<std::setw(5)<<samples[i].first<<"|"<<samples[i].second<<std::endl; 
	}
	
	delete[] hashes; 
}

