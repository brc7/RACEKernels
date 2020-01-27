#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <unordered_map>
#include <vector>

typedef unsigned int rangeless_race_sketch_t;

class RangelessRACE 
{
public:

    RangelessRACE(size_t R) : tables(R) {_R = R;}; 

    // ~RangelessRACE();

    void add(int* hashes); 
    void clear(); 
    size_t getR(); 

    double query(int *hashes);
    double query(int *hashes, size_t maxR); 
    size_t size(); 
    size_t size(size_t maxR); 


    void serialize(std::ostream &out); 
    void deserialize(std::istream &in); 

    void pprint(std::ostream& out, int width = 3, bool format = true); 
    
    private:
        size_t _R;
        std::vector< std::unordered_map<int, rangeless_race_sketch_t > > tables; 

        const uint8_t magic_number = 0x4A; // magic number for binary file IO
        const uint8_t file_version_number = 0x01; // file version number 
};




