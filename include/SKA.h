#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <utility>
#include <random>
#include <limits>


#include <math.h>

#include "Eigen/Core"
#include "Eigen/Dense"

#include "util.h"

/*
Implementation of Sparse Kernel Approximation method for KDE
*/

class SKA
{
public: 

    std::vector<size_t> samples(const std::vector<std::vector<double> > &dataset, size_t sketch_size, size_t initial_center = 0);
    std::vector<double> weights(double* kernel_matrix, double* samples_kde, size_t sketch_size);

    // http://dovgalecs.com/blog/eigen-how-to-get-in-and-out-data-from-eigen-matrix/
    // kernel matrix is a matrix K where K(i,j) = kernel function (samples[i], samples[j])
    // K is of size (sketch_size,sketch_size)
    // weights is a vector w of length sketch_size where w(i) = KDE(samples[i])
    
private:

};


