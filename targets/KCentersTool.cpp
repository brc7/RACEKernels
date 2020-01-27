#include "util.h"
#include "io.h"

#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <limits>
#include <vector>

/* Command line tool to find the greedy k-centers sketch of a dataset. 
*/


int main(int argc, char **argv){

	if (argc < 3){
		std::clog<<"Usage: "<<std::endl;
		std::clog<<"KCentersTool <data> <n_centers>"<<std::endl; 
		std::clog<<"--distanceID <int> (default 0)"<<std::endl; 
		std::clog<<"--skipD <int> (skips first columns in data vector, default 0)"<<std::endl; 
		std::clog<<"Supported distanceIDs:"<<std::endl;
		std::clog<<"        0: L2 (default)"<<std::endl;
		std::clog<<"        1: L1 "<<std::endl;
		std::clog<<"        2: Angular distance"<<std::endl;
		return 0;
	}

	std::ifstream datastream(argv[1]);
	size_t n_centers = std::stoi(argv[2]);

	// find optional arguments
	size_t skipD = 0; 
	size_t distanceID = 0; 
	bool isDense = false, isCSV = false; 

	for (int i = 3; i < argc; ++i){
		if (std::strcmp("--skipD",argv[i]) == 0){
			if ((i+1) < argc){
				skipD = std::stoi(argv[i+1]);
			} else {return -1;}
		}
		if (std::strcmp("--distanceID",argv[i]) == 0){
			if ((i+1) < argc){
				distanceID = std::stoi(argv[i+1]);
			} else {return -1;}
		}
	}

	// only supports libsvm format (sorry, deadlines)
    std::unordered_map<size_t, double> map;
    std::clog<<"Constructing K centers with K = "<<n_centers<<std::endl; 
    std::vector< std::unordered_map<size_t, double> > centers; 

	centers.reserve(n_centers);

	for (int i = 1; i < n_centers; i++){

		double max_distance = 0; 
	    std::unordered_map<size_t, double> candidate;

	    size_t DataID = 0; 
	    size_t CID = 0; 
	    std::clog<<"Finding center "<<i<<" of "<<n_centers<<": "; 


	    // iterate through the dataset for each of the K centers
	    do{
	    	bool success = SparseMapFeatures(datastream,map,skipD); 
			if (!success){
				DataID++; 
				continue;
			}

			if (centers.size() == 0){
				// if this is the first point, keep it as a center
				centers.push_back(map); 
				std::cout<<CID<<" "; 
				for (auto const& coordinate: map){
					std::cout<<coordinate.first<<":"<<coordinate.second<<" "; 
				}
				std::cout<<std::endl;

			} else {
				// check if the new point in "map" is better than candidate
				double min_distance = std::numeric_limits<double>::max(); 
				// find minimum distance to a center 
				for (const std::unordered_map<size_t,double>& center : centers){
					double distance = 0; 
					switch(distanceID){
						case 0: distance = L2Distance(map,center); 
						break; 
						case 1: distance = L1Distance(map,center); 
						break; 
						case 2: 
							double ip = InnerProduct(map, center);
							double ratio = 0; 
							if (ip == 0){
								ratio = 0.0; 
							} else {
								ratio = ip/(L2Norm(map) * L2Norm(center)); 
							}
							ratio = std::min(std::max(ratio,-1.0),1.0); 
							distance = std::acos(ratio); 
						break; 
					}
					if (distance < min_distance)
						min_distance = distance; 
				}
				// if minimum distance to a center is larger than the 
				// candidate's distance then replace candidate with map 
				if (min_distance > max_distance){
					candidate = map; 
					max_distance = min_distance; 
					CID = DataID; 
				}
			}
	        if (DataID%10000==0)
	            std::clog<<'.'<<std::flush;
	        DataID++;
	    }
	    while(datastream);
		std::clog<<std::endl;

	    datastream.clear();
	    datastream.seekg(0, std::ios::beg);

		std::cout<<CID<<" "; 
		for (auto const& coordinate: candidate){
			std::cout<<coordinate.first<<":"<<coordinate.second<<" "; 
		}
		std::cout<<std::endl;

	    // add candidate to centers
	    centers.push_back(candidate); 

	}
}