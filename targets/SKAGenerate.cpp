#include "util.h"
#include "io.h"
#include "SKA.h"

#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>
#include <vector>

/* Command line tool to construct SKA sketches 
*/


int main(int argc, char **argv){

	if (argc < 6){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"KCentersTool <kcenters> <kernel_id> <bandwidth> <kernel_values> <sketch>"<<std::endl; 
		std::clog<<"kcenters: File output from KCentersTool"<<std::endl; 
		std::clog<<"kernel_id:0\t L2 LSH kernel\n         :1\t L1 LSH kernel";
		std::clog<<"\n         :2\t Cauchy\n         :3\t Inverse multiquadratic";
		std::clog<<"\n         :4\t Rational quadratic\n         :5\t Student T"<<std::endl;
		std::clog<<"\n         :6\t Angular kernel"<<std::endl;
		std::clog<<"bandwidth: nonzero float"<<std::endl;
		std::clog<<"kernel_values: File output from KDETool on KCentersTool output"<<std::endl; 
		std::clog<<"--multiple_samples int,int,int,int,int"<<std::endl; 
		std::clog<<"--skipD <int> (skips first columns in data vector, default 1)"<<std::endl; 
		return 0;
	}

	std::ifstream kcenter_stream(argv[1]);
	int kernel_id = std::stoi(argv[2]); 
	double sigma = std::stod(argv[3]); 
	std::ifstream kernel_stream(argv[4]); 

	// read KDE values
	std::vector<double> KDEs; 
	bool success = VectorFeatures(kernel_stream, KDEs); 
	if (!success){
		std::cerr<<" SKAGenerate error 1: Problem with KDE file "<<std::endl; 
	}

	std::vector<size_t> samples; 
	samples.push_back(KDEs.size()); 

	// find optional arguments
	size_t skipD = 1;
	for (int i = 3; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
			} else {return -1;}
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


	// read data vector values into "centers"
	std::vector< std::unordered_map<size_t,double> > centers;
    std::unordered_map<size_t, double> map;

	do{
		bool success = SparseMapFeatures(kcenter_stream,map,skipD); 
		if (!success){
 			continue;
		}
    	centers.push_back(map);
    }
    while(kcenter_stream);


    // now create the sketches

	SKA ska; 

    for(auto& n : samples){
    	if (n <= 0) continue; 
    	if (n > centers.size()) continue; 
		if (n > KDEs.size()) continue; 

    	double* K = new double[n*n]();
		double* kappa = new double[n]();

		for(size_t i = 0; i < n; i++){
			for(size_t j = 0; j < n; j++){
				double k = 0; 
	        	switch(kernel_id){
	        		case 0: k = lsh_kernel_L2(L2Distance(centers[i], centers[j]), sigma);
	        		break;
	        		case 1: k = lsh_kernel_L1(L1Distance(centers[i], centers[j]), sigma);
	        		break;
	        		case 2: k = cauchy(L2Distance(centers[i], centers[j]), sigma);
	        		break;
	        		case 3: k = inverse_multiquadric(L2Distance(centers[i], centers[j]), sigma);
	        		break;
	        		case 4: k = rational_quadratic(L2Distance(centers[i], centers[j]), sigma);
	        		break;
	        		case 5: k = student_t(L2Distance(centers[i], centers[j]), std::round(sigma));
	        		break;
					case 6:
						double ip = InnerProduct(centers[i], centers[j]);
						double ratio = 0; 
						if (ip == 0){
							ratio = 0.0; 
						} else {
							ratio = ip/(L2Norm(centers[i]) * L2Norm(centers[j])); 
						}
						ratio = std::min(std::max(ratio,-1.0),1.0); 
						k = angular_kernel(std::acos(ratio), std::round(sigma));
					break;
	        	}
				K[i*n + j] = k; //update K matrix
			}
		}
		// update kappa 
		for(size_t i = 0; i < n; i++){
			kappa[i] = KDEs[i];
		}

		std::vector<double> weights = ska.weights(K, kappa, n);
    	
    	// now save the stuff
    	std::string s = argv[5]; 
    	s.append("-");
    	s.append(std::to_string(n));
    	s.append(".svm"); 
		std::ofstream sketchstream(s);
		WriteVectorCSV(sketchstream,weights); 
		
		for (size_t i = 0; i < n; i++){
			sketchstream<<i<<" "; 
			for (auto const& coordinate: centers[i]){
				sketchstream<<coordinate.first<<":"<<coordinate.second<<" "; 
			}
			sketchstream<<std::endl; 
		}

		// clean up 
		delete[] K; 
		delete[] kappa;  
		sketchstream.close(); 
    }

}