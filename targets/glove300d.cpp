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
    std::ifstream data("exp/glove300d.data");
    std::ifstream queries("exp/glove300d.queries");

    std::vector<double> estimates;
    // ExperimentResult test_RACE(n_hashes, dimensions, w, hash_power, 
    // hash_range,data,queries,estimates)


    std::ofstream fileRACE("exp/glove300dRACE.csv");
    std::vector<size_t> RACE_reps = {10,50,100,500,1000,2500,5000,10000, 20000};//1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& reps : RACE_reps){
        ExperimentResult result = test_RACE(reps, 300, 50, 2, 10, data, queries, estimates);
        WriteCSVResults(fileRACE, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream fileHBS("exp/glove300dHBS.csv");
    std::vector<size_t> HBS_samples = {5, 10, 50, 100, 500, 1000, 2500, 5000};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : HBS_samples){
        ExperimentResult result = test_HBS(n_samples, 300, 50, 2, 20.0, data, queries, estimates); 
        WriteCSVResults(fileHBS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream fileRS("exp/glove300dRS.csv");
    std::vector<size_t> RS_samples = {5, 10, 25, 50, 100, 150, 250};//, 5000, 8000, 10000, 15000, 20000};

    for (size_t& n_samples : RS_samples){
        ExperimentResult result = test_RS(n_samples, 300, 20.0, data, queries, estimates); 
        WriteCSVResults(fileRS, result.sketch_size, result.preprocessing_time, result.query_time, estimates); 
    }

    std::ofstream fileSKA("exp/glove300dSKA.csv"); 
    std::vector<size_t> SKA_samples = {5, 10, 100, 1000, 2500, 5000}; 

    for (size_t& n_samples : SKA_samples){
        ExperimentResult result = test_SKA(n_samples, 300, 20.0, data, queries, estimates); 
        WriteCSVResults(fileSKA,result.sketch_size,result.preprocessing_time,result.query_time,estimates); 
    }


    // ExperimentResult result = test_RS(100, 54, 200, data, queries, estimates); 
    // ExperimentResult result = test_SKA(150, 54, 200, data, queries, estimates); 


    // ExperimentResult test_HBS(size_t n_samples, size_t dimensions, int w, int hash_power, double sigma, std::ifstream& data, std::ifstream& queries, std::vector<double>& estimates);
    // ExperimentResult result = test_HBS(5000, 54, 501, 2, 200, data, queries, estimates); 

    // 1. Random sampling 

    // 2. RACE

    // 3. SKA

    // 4. HBE 

    return 0; 

}
