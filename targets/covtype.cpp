#include "RACE.h"
#include "L2Hash.h"
#include "MurmurHash.h"
#include "io.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "util.h"
#include "driver.h"

/*

Heuristics for choosing reps and range
Range increases as W decreases
Range increases at higher powers K 

Reps need to increase whenever sum p(x,q) becomes large 
- there should be a range-reps tradeoff
- 

*/


int main(){
    std::ifstream data("exp/covtype.data");
    std::ifstream queries("exp/covtype.queries");

    // std::ofstream covtypeSKA("exp/covtypeSKA.csv");
    // std::ofstream covtypeRS("exp/covtypeRS.csv");
    std::vector<double> estimates;
    
    std::ofstream covtypeRACE("exp/covtypeRACE.csv");
    std::vector<size_t> RACE_reps = {10, 25, 50, 100,250, 500, 1000, 5000};//, 500};//1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& reps : RACE_reps){
        ExperimentResult result = test_RACE(reps, 54, 501, 2, 10, data, queries, estimates);
        WriteCSVResults(covtypeRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream covtypeHBS("exp/covtypeHBS.csv");
    std::vector<size_t> HBS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : HBS_samples){
        ExperimentResult result = test_HBS(n_samples, 54, 501, 2, 200, data, queries, estimates); 
        WriteCSVResults(covtypeHBS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream covtypeRS("exp/covtypeRS.csv");
    std::vector<size_t> RS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : RS_samples){
        ExperimentResult result = test_RS(n_samples, 54, 200, data, queries, estimates); 
        WriteCSVResults(covtypeRS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream covtypeSKA("exp/covtypeSKA.csv");
    std::vector<size_t> SKA_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : SKA_samples){
        ExperimentResult result = test_SKA(n_samples, 54, 200, data, queries, estimates); 
        WriteCSVResults(covtypeSKA, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }
    return 0; 


    // ExperimentResult result = test_RS(100, 54, 200, data, queries, estimates); 
    // ExperimentResult result = test_SKA(150, 54, 200, data, queries, estimates); 


    // ExperimentResult test_HBS(size_t n_samples, size_t dimensions, int w, int hash_power, double sigma, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);
    // ExperimentResult result = test_HBS(5000, 54, 501, 2, 200, data, queries, estimates); 

    // 1. Random sampling 

    // 2. RACE

    // 3. SKA

    // 4. HBE 

}
