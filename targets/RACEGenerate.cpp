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
RACEGenerate <data> <dimensions> <n_samples> <sketch>
Optional arguments:
--hashID <N> (defaults to 0 - L2LSH)
--hashBinwidth <F> (defaults to 1.0)
--hashReps <N> (defaults to 1)
--nTables <N> (defaults to 5)
--skipD <N> (defaults to 0)
--skipQ <N> (defaults to 0)
--dense <N> (sets the isDense flag and uses dimensionality = N)

Later, you can query with the following command:
RACETool <sketch> <queries> <kernel_id> <bandwidth>
*/

int main(int argc, char **argv){

	if (argc < 5){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"RACEGenerate <data> <dimensions> <number_races> <sketch>"<<std::endl; 
		std::clog<<"--hashID <int> (default 0)"<<std::endl; 
		std::clog<<"--hashBinwidth <float> (default 1.0)"<<std::endl; 
		std::clog<<"--hashReps <int> (default 1)"<<std::endl; 
		std::clog<<"--skipD <int> (skips first columns in data vector, default 0)"<<std::endl; 
		std::clog<<"--dense (loads vector into memory in dense format)"<<std::endl; 
		std::clog<<"--csv (reads a csv file rather than libsvm format)"<<std::endl; 
		std::clog<<"Supported hashIDs:"<<std::endl; 
		std::clog<<"        0: L2 LSH (default)"<<std::endl; 
		std::clog<<"        1: L1 LSH "<<std::endl; 
		return 0;
	}

	std::ifstream datastream(argv[1]);
	size_t dimensions = std::stoi(argv[2]); 
	size_t number_races = std::stoi(argv[3]);
	std::ofstream sketchstream(argv[4], std::ios::binary);

	// find optional arguments
	size_t skipD = 0; 
	size_t hashID = 0, hashReps = 1; 
	double hashBinwidth = 1.0; 
	bool isDense = false, isCSV = false; 

	for (int i = 5; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
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
	}

    // file input buffer(s) - we pick which one based on isDense
    std::unordered_map<size_t, double> map;
    std::vector<double> vec; 

    std::clog<<"Creating LSH function"<<std::endl; 
    // TODO: choose LSH function based on config
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




    int* raw_hashes = new int[number_races*hashReps];
    int* rehashes = new int[number_races];
    std::clog<<"Constructing RangelessRACE"<<std::endl; 

    RangelessRACE sketch = RangelessRACE(number_races); 

    // put all the DataIDs into the RACEs tables
    size_t DataID = 0;
    do{
    	// parse datastream according to configs and deposit hashes in raw_hashes
    	if (!isDense){
    		bool success = SparseMapFeatures(datastream,map,skipD);
    		if (!success)continue; 
    		hash->getHash(map,raw_hashes); 
    	} else {
    		bool success; 
    		if (isCSV){
				success = VectorFeatures(datastream,vec,skipD);
    		} else {
    			success = SparseVectorFeatures(datastream,vec,dimensions,skipD);
    		}
    		if (!success)continue;
	    	hash->getHash(vec,raw_hashes);
    	}
        rehash(raw_hashes, rehashes, number_races, hashReps);
        sketch.add(rehashes); 

        if (DataID%10000==0)
            std::clog<<'.'<<std::flush;
        DataID++;
    }
    while(datastream);
	std::clog<<std::endl;

    sketch.serialize(sketchstream); 
}