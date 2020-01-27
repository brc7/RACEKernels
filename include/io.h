#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>

bool VectorFeatures(std::istream& in, std::vector<double>& vec); 
bool VectorFeatures(std::istream& in, std::vector<double>& vec, size_t skip); 
/*
Reads a CSV-coded set
element0, element1, element2, ... elementN
from stream "in"
Clears vec and and places element0, ... into vec
Skips the first "skip" elements in the CSV file
*/

void WriteVectorCSV(std::ostream& out, const std::vector<double>& values); 

void WriteCSVResults(std::ostream& out, size_t sketch_size, double preprocessing_time, double query_time, std::vector<double>& estimates); 
/* DEPRECATED 
Writes a CSV file row 
Assumes out starts at the start of the row (does not write leading newline)
Writes the following: 
[sketch_size in cells], [preprocessing time in seconds], [total querying time in seconds], KDE1, KDE2, KDE3, KDE4, (all doubles)

*/

/*
Probably faster to save as a 4MB array rather than an associative array so that's what we gonna do
*/
bool SparseVectorFeatures(std::istream& in, std::vector<double>& vec, size_t dimensionality);
bool SparseVectorFeatures(std::istream& in, std::vector<double>& vec, size_t dimensionality, size_t skip); 


bool SparseMapFeatures(std::istream& in, std::unordered_map<size_t,double>& map, size_t skip); 
bool SparseMapFeatures(std::istream& in, std::unordered_map<size_t,double>& map); 




