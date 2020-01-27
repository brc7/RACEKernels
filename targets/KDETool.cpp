#include "util.h"
#include "io.h"
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>


/* Command line tool to evaluate ground-truth KDEs. Usage: 
KDETool <data.libsvm> <queries.libsvm> <kernel_id> <bandwidth> <dimensions>

Optional flags:
--stream : don't load the entire dataset into memory
*/


int main(int argc, char **argv){

	if (argc < 6){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"KDETool <data> <queries> <kernel_id> <bandwidth> <dimensions> "<<std::endl;
		std::clog<<"kernel_id:0\t L2 LSH kernel\n         :1\t L1 LSH kernel";
		std::clog<<"\n         :2\t Cauchy\n         :3\t Inverse multiquadratic";
		std::clog<<"\n         :4\t Rational quadratic\n         :5\t Student T"<<std::endl;
		std::clog<<"\n         :6\t Angular kernel"<<std::endl;
		std::clog<<"bandwidth: nonzero float"<<std::endl;
		std::clog<<"(--skipD <N>): optional, skip the first N entries of each data vector row"<<std::endl;
		std::clog<<"(--skipQ <N>): optional, skip the first N entries of each query vector row"<<std::endl;
		return 0;
	}

	std::ifstream datastream(argv[1]);
	std::ifstream querystream(argv[2]);

	int kernel_id = std::stoi(argv[3]); 
	double sigma = std::stod(argv[4]);
	int dimensions = std::stoi(argv[5]);

	// find optional arguments
	size_t skipD = 0;
	size_t skipQ = 0;
	for (int i = 6; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
			}
		}
		if (std::strcmp("--skipQ",argv[i]) == 0){
			if ((i+1) < argc){
				skipQ = std::stoi(argv[i+1]);
			}
		}
	}

    // file input buffer
    std::unordered_map<size_t, double> vec;

	// first load all of the queries into RAM
	std::vector<std::unordered_map<size_t, double> > queries;
	queries.reserve(1000);
	do{
		bool success = SparseMapFeatures(querystream,vec,skipQ); 
    	if (!success)continue;
    	queries.push_back(vec); 
	} while(querystream); 

	std::vector<double> KDEs;
	KDEs.resize(queries.size()); // inits all to zero
	// stream the data and compute the KDEs for all the queries
    size_t DataID = 0; 
    do{
    	bool success = SparseMapFeatures(datastream,vec,skipD);
    	if (!success) continue; 
        if (DataID%10000==0)
            std::clog<<'.'<<std::flush;
        DataID++;
        #pragma omp parallel for 
        for(size_t i = 0; i < queries.size(); ++i){
        	double k = 0; 
        	switch(kernel_id){
        		case 0: k = lsh_kernel_L2(L2Distance(vec, queries[i]), sigma);
        		break;
        		case 1: k = lsh_kernel_L1(L1Distance(vec, queries[i]), sigma);
        		break;
        		case 2: k = cauchy(L2Distance(vec, queries[i]), sigma);
        		break;
        		case 3: k = inverse_multiquadric(L2Distance(vec, queries[i]), sigma);
        		break;
        		case 4: k = rational_quadratic(L2Distance(vec, queries[i]), sigma);
        		break;
        		case 5: k = student_t(L2Distance(vec, queries[i]), std::round(sigma));
        		break;
				case 6:
					double ip = InnerProduct(vec, queries[i]);
					double ratio = 0; 
					if (ip == 0){
						ratio = 0.0; 
					} else {
						ratio = ip/(L2Norm(vec) * L2Norm(queries[i])); 
					}
					ratio = std::min(std::max(ratio,-1.0),1.0); 
					k = angular_kernel(std::acos(ratio), std::round(sigma));
				break;
        	}
        	KDEs[i] += k;
        }
    }
    while(datastream);
    std::clog<<std::endl;
    
	double dataset_size = DataID; 
    for (size_t i = 0; i < KDEs.size(); ++i){
    	std::cout<<KDEs[i]/dataset_size;
    	if (i != KDEs.size() - 1){
    		std::cout<<','; 
    	}
    }
    std::cout<<std::endl;
}