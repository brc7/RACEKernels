#include "RangelessRACE.h"

// exclusively for pprint and serializers
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string> 

#include <cstdint>
#include <cstring>

#include <limits>



void RangelessRACE::add(int* hashes){
	// #pragma omp parallel
	for(size_t r = 0; r < _R; r++){
		tables[r][hashes[r]] += 1; 
		// auto iterator = tables[r].find(hashes[r]);
		// if (iterator == tables[r].end())
			// tables[r][hashes[r]] = 1;
		// else
			// iterator->second = iterator->second + 1; 
	}
}

size_t RangelessRACE::getR(){
	return _R; 
}

void RangelessRACE::clear(){
	for(size_t i = 0; i < _R; i++){
		tables[i].clear();
	}
}

double RangelessRACE::query(int *hashes){
	double mean = 0; 
	for(size_t r = 0; r < _R; r++){
		auto iterator = tables[r].find(hashes[r]); 
		if (iterator != tables[r].end())
			mean += iterator->second; 
	}
	mean = mean / _R; 
	return mean; 
}

double RangelessRACE::query(int *hashes, size_t maxR){
	if (maxR > _R){
		maxR = _R;
	}

	double mean = 0; 
	for(size_t r = 0; r < maxR; r++){
		auto iterator = tables[r].find(hashes[r]); 
		if (iterator != tables[r].end())
			mean += iterator->second; 
	}
	mean = mean / maxR; 
	return mean; 
}



size_t RangelessRACE::size(){
	size_t s = 0; 
	for(size_t r = 0; r < _R; r++){
		s+= 2*tables[r].size();
	}
	return s; 
}

size_t RangelessRACE::size(size_t maxR){
	if (maxR > _R){
		maxR = _R;
	}
	size_t s = 0; 
	for(size_t r = 0; r < maxR; r++){
		s+= 2*tables[r].size();
	}
	return s; 
}


void RangelessRACE::pprint(std::ostream& out, int width, bool format){

	for(size_t i = 0; i < _R; ++i){
		out << "------------" << std::endl; 
		out << "| Table "<<std::setw(3)<<i<<"|"<<std::endl; 
		out << "------------" << std::endl; 
		out << "| Key | N |"<<std::endl; 
		out << "------------" << std::endl; 

		std::vector<int> keys; 

		for (auto iterator : tables[i]){
			// for each bucket in the table, print out the contents 
			int key = iterator.first; 
			keys.push_back(key); 
		}
		std::sort (keys.begin(), keys.end());
		for(auto k : keys){
			out << std::setw(5) << k << "| " << tables[i][k]<<std::endl;
		}
	}
}
























void RangelessRACE::serialize(std::ostream &out){
	/*
	Input: A BINARY ostream out. You can open a binary output stream
	with the flag: std::ios::binary | std::ios::out
	Format: (Big Endian) 
	magic_number (uint8_t); file_version_number (uint8_t); R (uint64_t); range (uint64_t);
	Then every element in the sketch, as a uint32_t
	*/

	// 0. Determine if system is big-endian or little-endian 
	// 1. Write magic numbers and parameters to file 
	// 2. For each element in sketch, cast element to standard-sized element 
	// 3. Depending on (1), bitflip so that it is big-endian (network order)
	// 4. Write to stream 

	// 1. Determine if system is big-endian or little-endian
	uint32_t n = 1; 
	bool is_little_endian = *(uint8_t*)(&n);

	// 0. Write magic number and file version. Dont worry about endianness, as these are bytes 
	out.write(reinterpret_cast <const char *>(&magic_number), sizeof(uint8_t));
	out.write(reinterpret_cast <const char *>(&file_version_number), sizeof(uint8_t));

	// 0. Write the parameters to the file 
	uint64_t sketch_param = _R; 
	if (is_little_endian)
		sketch_param = __builtin_bswap64(sketch_param);
    out.write(reinterpret_cast <char *>(&sketch_param), sizeof(uint64_t));

    // 2. For each table in the RangelessRACE: 
    for (size_t r = 0; r < _R; r++){
    	// Write the number of items in the unordered_map 
    	uint32_t nItems = tables[r].size(); 
    	// Bitflip to be big-endian if needed 
    	if (is_little_endian)
    		nItems = __builtin_bswap32(nItems); 
    	// write to stream 
    	out.write(reinterpret_cast <char *>(&nItems), sizeof(int32_t)); 

    	for(const auto& x : tables[r]){
    		// write the map key
    		int32_t key = x.first; 
    		uint32_t value = x.second; 
    		if (is_little_endian){
    			key = __builtin_bswap32(key); 
    			value = __builtin_bswap32(value); 
    		}
    		out.write(reinterpret_cast <char *>(&key), sizeof(int32_t)); 
    		out.write(reinterpret_cast <char *>(&value), sizeof(uint32_t)); 
    	}
    }
}


void RangelessRACE::deserialize(std::istream& in){
  	/*   
	Input: A BINARY istream in. You can open a binary input stream
	with the flag: std::ios::binary | std::ios::in

	beware: frees sketch
	*/

	// 0. Determine if system is little-endian
	uint32_t n = 1;
	bool is_little_endian = *(uint8_t*)(&n);

	uint8_t magic, version;
	uint64_t R;

	in.read(reinterpret_cast<char *>(&magic), sizeof(uint8_t)); 
	in.read(reinterpret_cast<char *>(&version), sizeof(uint8_t)); 
	
	if (magic != magic_number)
		return;

	in.read(reinterpret_cast<char *>(&R), sizeof(uint64_t));

	// fix endian-ness of parameters
	if (is_little_endian){
		R = __builtin_bswap64(R); 
	}

	_R = R;

	uint32_t nItems = 0;
	tables.resize(_R); 
	for(size_t r = 0; r < _R; r++){
		tables[r].clear(); 
		in.read(reinterpret_cast<char *>(&nItems), sizeof(uint32_t)); 
		if (is_little_endian)
			nItems = __builtin_bswap32(nItems); 
			std::unordered_map<int, rangeless_race_sketch_t > map; 
		for (size_t i = 0; i < nItems; i++){
			// read nItems pairs of <int32_t,uint32_t> from the stream
			int32_t key = 0; 
			uint32_t value = 0; 
			in.read(reinterpret_cast <char *>(&key), sizeof(int32_t)); 
			in.read(reinterpret_cast <char *>(&value), sizeof(uint32_t)); 
			if (is_little_endian){
				key = __builtin_bswap32(key);
				value = __builtin_bswap32(value);  
			}
			tables[r][key] = value; 
		}
	}
}

