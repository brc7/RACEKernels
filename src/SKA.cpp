#include "SKA.h"

#include <iostream>
/*

The 2-Approximate Greedy Algorithm:
1) Choose the first center arbitrarily.

2) Choose remaining k-1 centers using the following criteria.
	Let c1, c2, c3, … ci be the already chosen centers. Choose
	the i+1 th center by picking the point which is furthest from 
	all of the selected centers. That is pick the point which maximizes X, where
	X = min(dist(p,c1),dist(p,c2), dist(p,c3)...)

*/

std::vector<size_t> SKA::samples(const std::vector<std::vector<double> > &dataset, size_t sketch_size, size_t initial_center){
	// to obtain sketch_size samples, we use the 2-approximate greedy kcenters algorithm
	size_t dataset_size = dataset.size(); 

	std::vector<size_t> centers; 

	centers.reserve(sketch_size);
	centers.push_back(initial_center);

	for (size_t i = 1; i < sketch_size; i++){
        std::cout<<'.'<<std::flush;

		size_t max_index = 0; 
		double max_distance = 0; 

		for (size_t j = 0; j < dataset_size; j++) {
			double min_distance = std::numeric_limits<double>::max(); 
			for (const size_t &center : centers){
				double dist = L2Distance(dataset[center], dataset[j]);
				if (dist < min_distance)
					min_distance = dist; 
			}

			if (min_distance > max_distance){
				max_distance = min_distance; 
				max_index = j; 
			}
		}
		centers.push_back(max_index);
	}
	return centers;
}


std::vector<double> SKA::weights(double* kernel_matrix, double* samples_kde, size_t sketch_size){
	// to find the weights we solve a linear algebra equation 
	// kernel_matrix * weights = samples_kde

	Eigen::MatrixXd K = Eigen::Map<Eigen::MatrixXd>(kernel_matrix, sketch_size, sketch_size); 
	Eigen::VectorXd kappa = Eigen::Map<Eigen::VectorXd>(samples_kde, sketch_size); 

	Eigen::VectorXd w = K.colPivHouseholderQr().solve(kappa);

	std::vector<double> weights; 
	weights.reserve(sketch_size); 

	// kludge-y way of getting data in and out of Eigen - sorry!
	// but I really wanted to keep things simple and not push the 
	// dependency on the linear algebra library into the calling code

	for(size_t i = 0; i < sketch_size; i++)
		weights.push_back(w(i)); 
	return weights; 
}

