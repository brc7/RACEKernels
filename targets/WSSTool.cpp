#include "util.h"
#include "io.h"
#include "HBS.h"
#include "L2Hash.h"
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>

/* Command line tool to read a weighted sample sketch (WSSTool). Usage:
WSSTool <sketch> <queries> <kernel_id> <bandwidth>

*/


int main(int argc, char **argv){

	if (argc < 5){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"WSSTool <sketch> <queries> <kernel_id> <bandwidth> "<<std::endl;
		std::clog<<"kernel_id:0\t L2 LSH kernel\n         :1\t L1 LSH kernel";
		std::clog<<"\n         :2\t Cauchy\n         :3\t Inverse multiquadratic";
		std::clog<<"\n         :4\t Rational quadratic\n         :5\t Student T";
		std::clog<<"\n         :6\t Angular kernel"<<std::endl;
		std::clog<<"bandwidth: nonzero float"<<std::endl;
		std::clog<<"(--skipD <N>): optional, skip the first N entries of each sketch vector row"<<std::endl;
		std::clog<<"(--skipQ <N>): optional, skip the first N entries of each query vector row"<<std::endl;
		return 0;
	}

	std::ifstream sketchstream(argv[1]);
	std::ifstream querystream(argv[2]);
	int kernel_id = std::stoi(argv[3]); 
	double sigma = std::stod(argv[4]);

	// find optional arguments
	size_t skipD = 0, skipQ = 0; 
	bool isCSV = false, isDense = false; 
	for (int i = 0; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--skipQ",argv[i]) == 0){
			if ((i+1) < argc){
				skipQ = std::stoi(argv[i+1]);
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

	std::vector<double> weights; 
	VectorFeatures(sketchstream, weights); 

    // file input buffer
    std::unordered_map<size_t, double> vec;

	// first load the sketch into RAM
	std::vector<std::unordered_map<size_t, double> > samples;
	do{
		bool success = SparseMapFeatures(sketchstream,vec,skipD); 
    	if (!success)break; 
    	samples.push_back(vec); 
	} while(sketchstream); 

	// Then print out the memory requirement
	size_t memory = 0; 
	for (size_t i = 0; i < samples.size(); i++){
		memory += 2*samples[i].size(); 
	}
	std::cout<<memory<<" sketch memory units"<<std::endl;


	if (samples.size() != weights.size()){
		std::cerr<<"WSSTool error 1"<<std::endl; 
		return -1; 
	}
	// then load the queries into RAM 
	std::vector<std::unordered_map<size_t, double> > queries;
	do{
		bool success = SparseMapFeatures(querystream,vec,skipQ); 
    	if (!success){break;}
    	queries.push_back(vec); 
	} while(querystream); 

	// now for every query, compute the weighted KDE
	std::vector<double> KDEs;
	KDEs.resize(queries.size()); // inits all to zero
	for(size_t i = 0; i < queries.size(); ++i){
        if (i%100==0)
            std::clog<<'.'<<std::flush;
        for(size_t j = 0; j < samples.size(); ++j){
        	double k = 0; 
        	switch(kernel_id){
        		case 0: k = weights[j]*lsh_kernel_L2(L2Distance(samples[j], queries[i]), sigma);
        		break;
        		case 1: k = weights[j]*lsh_kernel_L1(L1Distance(samples[j], queries[i]), sigma);
        		break;
        		case 2: k = weights[j]*cauchy(L2Distance(samples[j], queries[i]), sigma);
        		break;
        		case 3: k = weights[j]*inverse_multiquadric(L2Distance(samples[j], queries[i]), sigma);
        		break;
        		case 4: k = weights[j]*rational_quadratic(L2Distance(samples[j], queries[i]), sigma);
        		break;
        		case 5: k = weights[j]*student_t(L2Distance(samples[j], queries[i]), std::round(sigma));
        		break;
				case 6:
					double ip = InnerProduct(samples[j], queries[i]);
					double ratio = 0; 
					if (ip == 0){
						ratio = 0.0; 
					} else {
						ratio = ip/(L2Norm(samples[j]) * L2Norm(queries[i])); 
					}
					ratio = std::min(std::max(ratio,-1.0),1.0); 
					k = weights[j]*angular_kernel(std::acos(ratio), std::round(sigma));
				break;
        	}
        	KDEs[i] += k;
        }
	}
	std::clog<<std::endl; 

    for (size_t i = 0; i < KDEs.size(); ++i){
    	std::cout<<KDEs[i]; 
    	if (i != KDEs.size() - 1){
    		std::cout<<','; 
    	}
    }
    std::cout<<std::endl;

}


