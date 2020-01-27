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
    std::ifstream data("exp/mnist.data");
    std::ifstream queries("exp/mnist.queries");
    
    std::vector<double> estimates;

    std::ofstream fileRangelessRACE("exp/mnistRangelessRACE.csv");
    std::vector<size_t> Rangeless_reps = {10, 25, 50, 100, 250, 500, 1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

// sigma = 2000.0
// ww = 5000

    for (size_t& reps : Rangeless_reps){
        ExperimentResult result = test_RangelessRACE(reps, 784, 5000, 2, data, queries, estimates);
        WriteCSVResults(fileRangelessRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }


    std::ofstream fileHBS("exp/mnistHBS.csv");
    std::vector<size_t> HBS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : HBS_samples){
        ExperimentResult result = test_HBS(n_samples, 784, 5000, 2, 2000.0, data, queries, estimates); 
        WriteCSVResults(fileHBS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream fileRS("exp/mnistRS.csv");
    std::vector<size_t> RS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : RS_samples){
        ExperimentResult result = test_RS(n_samples, 784, 2000.0, data, queries, estimates); 
        WriteCSVResults(fileRS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream fileSKA("exp/mnistSKA.csv");
    std::vector<size_t> SKA_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : SKA_samples){
        ExperimentResult result = test_SKA(n_samples, 784, 2000.0, data, queries, estimates); 
        WriteCSVResults(fileSKA, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    return 0;

}

    // std::ofstream fileRACE("exp/mnistRACE.csv");
    // std::vector<size_t> RACE_reps = {100, 250, 500, 1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

    // for (size_t& reps : RACE_reps){
    //     ExperimentResult result = test_RACE(reps, 784, 2000, 2, 1000, data, queries, estimates);
    //     WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    // }

    // for (size_t& reps : RACE_reps){
    //     ExperimentResult result = test_RACE(reps, 784, 2000, 2, 75, data, queries, estimates);
    //     WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    // }

    // for (size_t& reps : RACE_reps){
    //     ExperimentResult result = test_RACE(reps, 784, 2000, 2, 100, data, queries, estimates);
    //     WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    // }

    // for (size_t& reps : RACE_reps){
    //     ExperimentResult result = test_RACE(reps, 784, 2000, 2, 500, data, queries, estimates);
    //     WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    // }
