#pragma once 

#include "io.h"
#include "util.h"
#include "RACE.h"
#include "SKA.h"
#include "HBS.h"
#include "RangelessRACE.h"

#include "L2Hash.h"
#include "MurmurHash.h"


#include <vector>
#include <iostream>
#include <fstream>

#include <chrono>


/*
Driver for the experiments that we will run

Basically, this just wraps RACE, RS, SKA, HBE 
and any other method we might choose to implement
in an easy-to-call function that conducts the experiment
and deposits the results in a format that is easy to deal with
*/


// convenience struct for packaging data we want 
// to plot or analyze later in experiments 
struct ExperimentResult
{
    size_t sketch_size; 
    double preprocessing_time;
    double query_time;
};

ExperimentResult test_RACE(size_t n_hashes, size_t dimensions, int w, int hash_power, size_t hash_range, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);
ExperimentResult test_HBS(size_t n_samples, size_t dimensions, int w, int hash_power, double sigma, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);

ExperimentResult test_RangelessRACE(size_t n_hashes, size_t dimensions, int w, int hash_power, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);

// n_hashes is the number of RACE repetitions
// dimensions is dimensionality of dataset 
// w is the value of w used internally in the L2Hash 
// hash_power is the number of repeated hash function (in most papers, K). It determines p^K, where p is collision probability 
// hash_range is the range of each RACE repetition 
// data is a file with data, ready to be read 
// queries is a file with data, ready to be read 
// estimates is a vector that will be populated with the KDE of all the queries in the query file 


ExperimentResult test_RS(size_t n_samples, size_t dimensions, double sigma, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);
ExperimentResult test_SKA(size_t n_samples, size_t dimensions, double sigma, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);

