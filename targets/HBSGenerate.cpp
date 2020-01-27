#include "util.h"
#include "io.h"
#include "HBS.h"
#include "L2Hash.h"
#include "L1Hash.h"
#include "SRPHash.h"
#include "Hash.h"

#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>
#include <vector>
#include <sstream>

/* Command line tool to generate a hash-based sketch (HBS). Usage:

Desired interface:
HBSGenerate <data> <n_samples> <sketch>
Optional arguments:
--hashID <N> (defaults to 0 - L2LSH)
--hashBinwidth <F> (defaults to 1.0)
--hashReps <N> (defaults to 1)
--nTables <N> (defaults to 5)
--skipD <N> (defaults to 0)
--skipQ <N> (defaults to 0)
--dense <N> (sets the isDense flag and uses dimensionality = N)

Later, you can query with the following command:
HBSTool <sketch> <queries> <kernel_id> <bandwidth>
*/


int main(int argc, char **argv){

	if (argc < 5){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"HBSGenerate <data> <dimensions> <n_samples> <sketch>"<<std::endl; 
		std::clog<<"--multiple_samples int,int,int,int,int"<<std::endl; 
		std::clog<<"--hashID <int> (default 0)"<<std::endl; 
		std::clog<<"--hashBinwidth <float> (default 1.0)"<<std::endl; 
		std::clog<<"--hashReps <int> (default 1)"<<std::endl; 
		std::clog<<"--nTables <int> (default 5)"<<std::endl; 
		std::clog<<"--seed <unsigned int> (default 42)"<<std::endl; 
		std::clog<<"--skipD <int> (skips first columns in data vector, default 0)"<<std::endl; 
		std::clog<<"--dense (loads vector into memory in dense format)"<<std::endl; 
		std::clog<<"--csv (reads a csv file rather than libsvm format)"<<std::endl; 
		std::clog<<"Supported hashIDs:"<<std::endl; 
		std::clog<<"        0: L2 LSH (default)"<<std::endl; 
		std::clog<<"        1: L1 LSH "<<std::endl; 
		std::clog<<"        2: SRP "<<std::endl; 
		return 0;
	}

	std::ifstream datastream(argv[1]);
	size_t dimensions = std::stoi(argv[2]); 
	size_t n_samples = std::stoi(argv[3]);
	std::vector<size_t> samples; 
	samples.push_back(n_samples); 

	// find optional arguments
	size_t skipD = 0, nTables = 5; 
	size_t hashID = 0, hashReps = 1; 
	double hashBinwidth = 1.0; 
	bool isDense = false, isCSV = false; 
	unsigned int seed = 42; 

	for (int i = 5; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--nTables",argv[i]) == 0){
			if ((i+1) < argc){
				nTables = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--hashID",argv[i]) == 0){
			if ((i+1) < argc){
				hashID = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--seed",argv[i]) == 0){
			if ((i+1) < argc){
				seed = std::stoi(argv[i+1]);
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
		if (std::strcmp("--multiple_samples",argv[i]) == 0){
			if ((i+1) < argc){
				std::stringstream ss(argv[i+1]); 
				size_t element = 0; 
				while(ss >> element){
					samples.push_back(element); 
					if (ss.peek() == ',')
						ss.ignore(); 
				}
			} else {return -1;} 
		}
	}

    // file input buffer(s) - we pick which one based on isDense
    std::unordered_map<size_t, double> map;
    std::vector<double> vec; 

    int* hashes = new int[nTables]; 
    HBS sketch = HBS(nTables); 

    std::clog<<"Constructing LSH function"<<std::endl; 
    // TODO: choose LSH function based on config
    // FINALLY FIXED THIS 

	// L2Hash hash = L2Hash(dimensions, nTables*hashReps, hashBinwidth);

    Hash* hash;
    switch(hashID){
    	case 0: 
			hash = new L2Hash(dimensions, nTables*hashReps, hashBinwidth, seed); 
			break; 
    	case 1: 
    		hash = new L1Hash(dimensions, nTables*hashReps, hashBinwidth, seed); 
    		break; 
    	case 2: 
    		hash = new SRPHash(dimensions, nTables*hashReps, seed);
    		break; 
    	default: 
    		hash = new L2Hash(dimensions, nTables*hashReps, hashBinwidth, seed); 
    }

    int* raw_hashes = new int[nTables*hashReps];
    int* rehashes = new int[nTables];
    std::clog<<"Constructing HBS sketch"<<std::endl; 


    // put all the DataIDs into the hash tables
    size_t DataID = 0;
    size_t dataset_size = 0; 
    do{
    	// parse datastream according to configs and deposit hashes in raw_hashes
    	if (!isDense){
    		bool success = SparseMapFeatures(datastream,map,skipD);
    		if (!success){
    			DataID++; 
    			continue;
    		} 
    		hash->getHash(map,raw_hashes); 
    	} else {
    		bool success; 
    		if (isCSV){
				success = VectorFeatures(datastream,vec,skipD);
    		} else {
    			success = SparseVectorFeatures(datastream,vec,dimensions,skipD);
    		}
    		if (!success){
    			DataID++; 
    			continue; 
    		}
	    	hash->getHash(vec,raw_hashes);
    	}

        rehash(raw_hashes, rehashes, nTables, hashReps);
        sketch.add(rehashes, DataID); 
        dataset_size++; 

        if (DataID%10000==0)
            std::clog<<'.'<<std::flush;
        DataID++;
    }
    while(datastream);
	std::clog<<std::endl;

	std::clog<<"Dataset size = "<<dataset_size<<std::endl; 

    for(auto& n : samples){
    	std::string s = argv[4]; 
    	s.append("-");
    	s.append(std::to_string(n));
    	s.append(".svm"); 
		std::ofstream sketchstream(s);

	    // find the selected indices and the weights from the HBS
	    std::vector<std::pair<size_t, double> > selected_indices = sketch.samples(n,dataset_size);
	    // sort the index, weight pairs in order of occurrence 
	    std::sort(selected_indices.begin(), selected_indices.end(), [](const std::pair<size_t,double> &left, const std::pair<size_t,double> &right) {
		    return left.first < right.first; });
	    // bugfix: there might be duplicates in the list of indices due to how 
	    // the HBS works, we forgot to include them

	    std::vector<double> weights;
	    for(auto selection : selected_indices){
	    	weights.push_back(selection.second*dataset_size);
	    	std::cout<<selection.first<<" : "<<selection.second<<std::endl; 
	    }

	    WriteVectorCSV(sketchstream,weights); 


		// reset datastream
	    datastream.clear();
	    datastream.seekg(0, std::ios::beg);

	    // Now save the selected set of samples
	    DataID = 0;
	    std::string line;
	    std::getline(datastream, line);

	    for (auto selection : selected_indices){
	    	size_t targetDataID = selection.first; 
	    	if (DataID != targetDataID){
	    		// then make it equal
		    	while ((DataID < targetDataID - 1) && (DataID < dataset_size)){
		    		// skip rows until we get to targetDataID - 1
		    		DataID++;
		    		datastream.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		    	}
		    	// now read into line
		    	std::getline(datastream, line);
		    	DataID++; 
	    	}
	    	// write this line to sketchstream
	    	sketchstream<<line<<std::endl; 
	    	std::string debug = line; 
	    	debug.resize(40); 
	    	std::clog<<DataID<<" : "<<debug<<std::endl; 
	    }

		sketchstream.close(); 
    }
}