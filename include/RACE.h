#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>


typedef unsigned int race_sketch_t;

class RACE 
{
public:
    RACE(size_t R, size_t range); 
    ~RACE(); 

    void add(int *hashes); 
    void subtract(int *hashes); 
    void clear(); 

    double query(int *hashes); 

    void serialize(std::ostream &out); 
    void deserialize(std::istream &in); 

    void pprint(std::ostream& out, int width = 3, bool format = true); 
    

    size_t max_counter(); 
    void delta_unzip(const size_t dataset_size); 
    void delta_zip(); 

    private:
        size_t _R, _range;
        race_sketch_t* _sketch;
        const uint8_t magic_number = 0x4D; // magic number for binary file IO
        const uint8_t file_version_number = 0x01; // file version number 
};




