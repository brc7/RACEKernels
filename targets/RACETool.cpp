#include "util.h"
#include "io.h"
#include "L2Hash.h"
#include "L1Hash.h"
#include "SRPHash.h"
#include "Hash.h"
#include "RangelessRACE.h"
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>

/* Command line tool to generate a hash-based sketch (HBS). Usage:

Desired interface:
RACETool <sketch> <queries>
Optional arguments:
--hashID <N> (defaults to 0 - L2LSH)
--hashBinwidth <F> (defaults to 1.0)
--hashReps <N> (defaults to 1)
--skipQ <N> (defaults to 0)
--dense <N> (sets the isDense flag and uses dimensionality = N)

*/

int main(int argc, char **argv){

	if (argc < 4){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"RACEGenerate <sketch> <dimensions> <queries>"<<std::endl; 
		std::clog<<"--multiple_races int,int,int,int,int..."<<std::endl; 
		std::clog<<"--hashID <int> (default 0)"<<std::endl; 
		std::clog<<"--hashBinwidth <float> (default 1.0)"<<std::endl; 
		std::clog<<"--hashReps <int> (default 1)"<<std::endl; 
		std::clog<<"--skipQ <int> (skips first columns in data vector, default 0)"<<std::endl; 
		std::clog<<"--dense (loads vector into memory in dense format)"<<std::endl; 
		std::clog<<"--csv (reads a csv file rather than libsvm format)"<<std::endl; 
		std::clog<<"Supported hashIDs:"<<std::endl; 
		std::clog<<"        0: L2 LSH (default)"<<std::endl; 
		std::clog<<"        1: L1 LSH "<<std::endl; 
		std::clog<<"        2: SRP "<<std::endl; 

		return 0;
	}

	std::ifstream sketchstream(argv[1], std::ios::binary);
	size_t dimensions = std::stoi(argv[2]); 
	std::ifstream querystream(argv[3]);

	std::vector<size_t> multiple_races;

	RangelessRACE sketch = RangelessRACE(0); 
	sketch.deserialize(sketchstream); 
	size_t number_races = sketch.getR(); 
	multiple_races.push_back(number_races); 

	// find optional arguments
	size_t skipQ = 0; 
	size_t hashID = 0, hashReps = 1; 
	double hashBinwidth = 1.0; 
	bool isDense = false, isCSV = false; 

	for (int i = 0; i < argc; ++i){
		if (std::strcmp("--skipQ",argv[i]) == 0){
			if ((i+1) < argc){
				skipQ = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--hashID",argv[i]) == 0){
			if ((i+1) < argc){
				hashID = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--hashReps",argv[i]) == 0){
			if ((i+1) < argc){
				hashReps = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--hashBinwidth",argv[i]) == 0){
			if ((i+1) < argc){
				hashBinwidth = std::stod(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--dense",argv[i]) == 0){
			isDense = true; 
		}
		if (std::strcmp("--csv",argv[i]) == 0){
			isDense = true; 
			isCSV = true; 
		}
		if (std::strcmp("--multiple_races",argv[i]) == 0){
			if ((i+1) < argc){
				std::stringstream ss(argv[i+1]); 
				size_t element = 0; 
				while(ss >> element){
					multiple_races.push_back(element); 
					if (ss.peek() == ',')
						ss.ignore(); 
				}
			} else {return -1;} 
		}

	}

	// L2Hash hash = L2Hash(dimensions, number_races*hashReps, hashBinwidth);

    Hash* hash;
    switch(hashID){
    	case 0: 
			hash = new L2Hash(dimensions, number_races*hashReps, hashBinwidth); 
			break; 
    	case 1: 
    		hash = new L1Hash(dimensions, number_races*hashReps, hashBinwidth); 
    		break; 
    	case 2: 
    		hash = new SRPHash(dimensions, number_races*hashReps);
    		break; 
    	default: 
    		hash = new L2Hash(dimensions, number_races*hashReps, hashBinwidth); 
    }

    // file input buffer(s) - we pick which one based on isDense
    std::unordered_map<size_t, double> vec;

	// first load all of the queries into RAM
	std::vector<std::unordered_map<size_t, double> > queries;
	queries.reserve(1000);
	do{
		bool success = SparseMapFeatures(querystream,vec,skipQ); 
    	if (!success)continue;
    	queries.push_back(vec); 
	} while(querystream); 


	// print out the Rs
	std::cout<<"RACE ranges: "<<std::endl;
	for(size_t i = 0; i < multiple_races.size(); i++){
		std::cout<<multiple_races[i]; 
		if (i != multiple_races.size() - 1){
			std::cout<<","; 
		}
	}
	std::cout<<std::endl; 

	// now print out how large each of the RACEs will be
	std::cout<<"RACE sizes: "<<std::endl;
	for(size_t i = 0; i < multiple_races.size(); i++){
		std::cout<<sketch.size(multiple_races[i]); 
		if (i != multiple_races.size() - 1){
			std::cout<<","; 
		}
	}
	std::cout<<std::endl; 

	// std::cout<<sketch.size()<<" sketch memory units"<<std::endl; 

	std::vector<std::vector<double> > KDEs; 
	for (size_t i = 0; i < multiple_races.size(); i++){
		std::vector<double> k; 
		k.resize(queries.size()); // inits all to zero
		KDEs.push_back(k); 
	}
	// compute the KDEs for all the queries
    #pragma omp parallel for 
    for(size_t i = 0; i < queries.size(); ++i){
        if (i%100==0)
            std::clog<<'.'<<std::flush;
	    int* raw_hashes = new int[number_races*hashReps];
	    int* rehashes = new int[number_races];
    	hash->getHash(queries[i],raw_hashes); 
        rehash(raw_hashes, rehashes, number_races, hashReps);
        
        for (size_t j = 0; j < multiple_races.size(); j++){
	        double estimate = sketch.query(rehashes, multiple_races[j]); 
	    	(KDEs[j])[i] = estimate; 
        }
    	delete[] raw_hashes; 
    	delete[] rehashes; 
    }
    std::clog<<std::endl; 

    for (size_t i = 0; i < KDEs.size(); ++i){
    	for (size_t j = 0; j < KDEs[i].size(); ++j){
	    	std::cout<<(KDEs[i])[j]; 
	    	if (j != KDEs[i].size() - 1){
	    		std::cout<<','; 
	    	}
    	}
		std::cout<<std::endl;
    }
}