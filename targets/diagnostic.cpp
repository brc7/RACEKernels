#include "RACE.h"
#include "L2Hash.h"
#include "MurmurHash.h"
#include "io.h"

#include <vector>
#include <iostream>
#include <chrono>

#include "util.h"

/*

Heuristics for choosing reps and range
Range increases as W decreases
Range increases at higher powers K 

Reps need to increase whenever sum p(x,q) becomes large 
- there should be a range-reps tradeoff
- 

*/


// void rehash(int* input_hashes, int* output_hashes, int nhashes, int values_per_set){
//     #pragma omp parallel for 
//     for (size_t i = 0; i < nhashes; i++){
//        output_hashes[i] = MurmurHash(input_hashes + values_per_set*i, sizeof(int)*values_per_set, 42);
//     }
// }

int main(){
/*
    std::ifstream data("exp/covtype.data");
    std::vector<double> vec;

    const size_t nhashes = 40000;
    const size_t range = 20;

    // N = 30,000
    // NH = 10k, R = 3
    // R = 1k, NH = 3k gives good results
    // R = 3, NH = 1 million (ugh)
    const size_t dim = 54;
    // const size_t nhashes = 10000; 

    int rehashes = 5; 
    
    L2Hash H1000_x4 = L2Hash(dim,nhashes*rehashes,1000);

    int* hashes_x4 = new int[nhashes*rehashes];
    int* hashes = new int[nhashes];

    RACE A1000_x4 = RACE(nhashes,range);

    auto start_time = std::chrono::high_resolution_clock::now();
    int idx = 0;
    do{
        VectorFeatures(data,vec);
        H1000_x4.getHash(vec,hashes_x4);
        rehash(hashes_x4, hashes, nhashes, rehashes);
        A1000_x4.add(hashes);
        if (idx%10000==0)
            std::cout<<'.'<<std::flush;
        idx++;
    }
    while(data);

    double time = (std::chrono::high_resolution_clock::now() - start_time).count() * 1e-9;

    std::cout<<std::endl<<"Preprocessing: "<<time<<" seconds"<<std::endl;

    std::ifstream queries("exp/covtype.queries");
    do{
        VectorFeatures(queries,vec);

        H1000_x4.getHash(vec,hashes_x4);
        rehash(hashes_x4, hashes, nhashes, rehashes);
        std::cout<<A1000_x4.query(hashes)<<',';
    }
    while(queries);
*/

}
