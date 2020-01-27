#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <utility>
#include <random>

#include <math.h>

/* Simple self-organizing maps implementation
For simplicity, only supports 2D maps

Should probably rename to SOMKE

*/


class SOM
{
public:
    SOM(size_t n_nodes, size_t data_dimension);

    void initialize();
    void update(std::vector<double>& vec);
    void count(std::vector<double>& vec);
    void samples();

private:
    size_t n; // allocates n x n grid of nodes
    size_t d; // each node has d dimensions
    size_t t; // number of elements inserted into SOM

    double* nodes;
    int* counts;

    double learning_rate(size_t time);
    double neighborhood_rate(size_t time, int x, int y, int xc, int yc);

    const unsigned int seed = 42; 

};


