#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <utility>
#include <random>


#include <math.h>



/*
Currently implemented as a 2-pass algorithm, to avoid loading
huge datasets into RAM

On the first pass, we figure out which points to use in the sketch
and we compute the weights associated with each point 

*/


class HBS
{
public: 

    struct bucket_info{
        int bucket_key;
        int bucket_size;  
        double bucket_p; 
        double bucket_w; 
    };

    HBS(size_t n_tables) : tables(n_tables) {num_tables = n_tables;}; 
    // num_tables = number of hash tables. In original paper, this was 5
    // Note that here, the table_range is data-dependent (we don't rehash to a finite range)

    // ~HBS();

    void add(int* hashes, size_t DataID);
    // hashes is a set of num_tables integer hashes
    // the integers are allowed to be any valid integer value

    std::vector<std::pair<size_t, double> > samples(size_t sketch_size, size_t dataset_size);

    void pprint(std::ostream& out, int width = 4, bool format = true);

    // sketch size = number of data elements to keep
    // returns a vector of length sketch_size
    // the vector contains pairs where
    // first element is an integer = DataID
    // second element is a double = weight associated with that DataID
    // (uses return value optimization, requires c++11)
    // return vector not guaranteed to be sorted 

private: 
    double compute_gamma(size_t table_number, size_t dataset_size); 

    const unsigned int seed = 42; 
    size_t num_tables;

    std::vector< std::unordered_map<int, std::vector<size_t> > > tables; 
    // this might look complicated, but it is really just a 
    // set of tables, where each table maps an integer value to a set of DataIDs
};


