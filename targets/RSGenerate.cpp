#include "util.h"
#include "io.h"
#include "HBS.h"
#include "L2Hash.h"
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>

/* Command line tool to generate a random sample sketch (RS). Usage:
RSGenerate <data> <n_samples> <sketch>
Later, you can query with the following command:
WSSTool <sketch> <queries> <kernel_id> <bandwidth>
*/


int main(int argc, char **argv){

	if (argc < 4){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"RSGenerate <data> <n_samples> <sketch>"<<std::endl; 
        std::clog<<"--seed: <unsigned int> (default 42)"<<std::endl; 
		return 0;
	}

    unsigned int seed = 42; 
    for (int i = 3; i < argc; ++i){
        if (std::strcmp("--seed",argv[i]) == 0){
            if ((i+1) < argc){
                seed = std::stoi(argv[i+1]);
            } else {return -1;}
        }
    }


	std::ifstream datastream(argv[1]);
	size_t n_samples = std::stoi(argv[2]);
	std::ofstream sketchstream(argv[3]);

	// simple implementation of reservoir sampling on strings (lines of the file)
	
	// unsigned int seed = 42;
	std::mt19937 generator(seed);

    std::vector< std::string > samples;
    samples.reserve(n_samples);
    
    std::string line; 
    int i = 0;
    do{
    	std::getline(datastream,line); 
    	// if (line.empty())continue; 
        if (line.find(":") == std::string::npos) continue; 

    	if (i < n_samples){
	    		samples.push_back(line);
	    } else {
    		std::uniform_int_distribution<int> distribution(0,i-1);
    		int j = distribution(generator); 
    		if (j < n_samples){
	    		samples[j] = line;
    		}
    	}

        if (i%10000==0)
            std::clog<<'.'<<std::flush;
        i++;
    }
    while(datastream);
    std::clog<<std::endl; 
    
    double dataset_size = i; 
    std::vector<double> weights(n_samples, 1.0/n_samples);
    
    WriteVectorCSV(sketchstream,weights); 
    for(const std::string& s : samples){
    	sketchstream<<s<<std::endl; 
    }

}


