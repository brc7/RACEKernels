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

sigma = 20.0

*/


int main(){



    std::ifstream data("exp/glove50d.data");
    std::ifstream queries("exp/glove50d.queries");

    // std::ofstream covtypeSKA("exp/covtypeSKA.csv");
    // std::ofstream covtypeRS("exp/covtypeRS.csv");

    std::vector<double> estimates;
    /*
    std::ofstream fileRACE("exp/glove50dRACE.csv");
    std::vector<size_t> RACE_reps = {10, 25, 50, 100, 250, 500, 1000, 5000};//, 500};//1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& reps : RACE_reps){
        ExperimentResult result = test_RACE(reps, 50, 20, 2, 10, data, queries, estimates);
        WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream covtypeHBS("exp/glove50dHBS.csv");
    std::vector<size_t> HBS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : HBS_samples){
        ExperimentResult result = test_HBS(n_samples, 50, 20, 2, 8.0, data, queries, estimates); 
        WriteCSVResults(fileHBS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }
    */
    std::ofstream covtypeRS("exp/glove50dRS.csv");
    std::vector<size_t> RS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : RS_samples){
        ExperimentResult result = test_RS(n_samples, 50, 8.0, data, queries, estimates); 
        WriteCSVResults(covtypeRS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream covtypeSKA("exp/glove50dSKA.csv");
    std::vector<size_t> SKA_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : SKA_samples){
        ExperimentResult result = test_SKA(n_samples, 50, 8.0, data, queries, estimates); 
        WriteCSVResults(covtypeSKA, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }
    return 0; 

}