#include "RACE.h"
#include "MurmurHash.h"

// exclusively for pprint and serializers
#include <iostream>
#include <iomanip>
#include <string> 

#include <cstdint>
#include <cstring>

#include <limits>


RACE::RACE(size_t R, size_t range){
	// parameters: R = number of ACE repetitions
	// range = size of each ACE array 
	_R = R, 
	_range = range; 

	_sketch = new race_sketch_t[ _R*_range ](); 
}

RACE::~RACE(){
	delete[] _sketch; 
}

void RACE::add(int *hashes){
	/* 
	Input: A set of R integer hash values, one hash value for each ACE repetition
	*/
	#pragma omp parallel for
	for (size_t r = 0; r < _R; r++){
		size_t index = hashes[r] % _range; 
		_sketch[r*_range + index] = _sketch[r*_range + index] + 1; 
	}
}

void RACE::subtract(int *hashes){
	/*
	Input: A set of R integer hash values, one hash value for each ACE repetition
	*/
	#pragma omp parallel for
	for (size_t r = 0; r < _R; r++){
		size_t index = hashes[r] % _range; 
		_sketch[r*_range + index] = _sketch[r*_range + index] - 1;
	}
}

double RACE::query(int *hashes){

	double mean = 0; 
	for (size_t r = 0; r < _R; r++){
		size_t index = hashes[r] % _range;
		mean = mean + _sketch[r*_range + index]; 
	}
	mean = mean / _R; 
	return mean; 
}


void RACE::clear(){
	memset(_sketch, 0, _R*_range*sizeof(*_sketch));
}



void RACE::delta_zip(){
/*
Performs online delta compression on the counters
*/
	#pragma omp parallel for
	for (size_t r = 0; r < _R; r++){
		// size_t index = hashes[r] % _range; 
		// _sketch[r*_range + index] = _sketch[r*_range + index] - 1;

		race_sketch_t min = std::numeric_limits<race_sketch_t>::max();
		for(size_t i = 0; i < _range; i++){
			if (_sketch[r*_range + i] < min)
				min = _sketch[r*_range + i];
		}
		for(size_t i = 0; i < _range; i++){
			_sketch[r*_range + i] = _sketch[r*_range + i] - min;
		}
	}

}

void RACE::delta_unzip(const size_t dataset_size){
/*
Decompresses the counters 
*/

	#pragma omp parallel for
	for (size_t r = 0; r < _R; r++){

		race_sketch_t sum = 0; 
		for(size_t i = 0; i < _range; i++){
			sum += _sketch[r*_range + i];
		}
		race_sketch_t difference = dataset_size - sum;
		for(size_t i = 0; i < _range; i++){
			_sketch[r*_range + i] = _sketch[r*_range + i] + difference;
		}
	}
}

size_t RACE::max_counter(){
// returns the value of the largest counter in the array 
	size_t max = 0; 
	for (size_t r = 0; r < _R*_range; r++){
		if (max < _sketch[r])
			max = (size_t)_sketch[r]; 
	}
	return max; 
}

















void RACE::serialize(std::ostream &out){
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
	
	sketch_param = _range; 
	if (is_little_endian)
		sketch_param = __builtin_bswap64(sketch_param);
    out.write(reinterpret_cast <char *>(&sketch_param), sizeof(uint64_t));

	// 2. For each element in sketch
	for (size_t r = 0; r < _R; r++){
		for (size_t i = 0; i < _range; i++){
			// 2. Cast element to standard-sized element
			uint32_t value = _sketch[r*_range + i];
			// 3. Depending on (1), bitflip so that it is big-endian (network order)
			if (is_little_endian)
				value = __builtin_bswap32(value);
			// 4. Write to stream 
			out.write(reinterpret_cast <char *>(&value), sizeof(uint32_t));
		}
	}
}


void RACE::deserialize(std::istream& in){
  	/*   
	Input: A BINARY istream in. You can open a binary input stream
	with the flag: std::ios::binary | std::ios::in

	beware: frees sketch
	*/

	// 0. Determine if system is little-endian
	uint32_t n = 1;
	bool is_little_endian = *(uint8_t*)(&n);

	uint8_t magic, version;
	uint64_t R, range;

	in.read(reinterpret_cast<char *>(&magic), sizeof(uint8_t)); 
	in.read(reinterpret_cast<char *>(&version), sizeof(uint8_t)); 
	
	if (magic != magic_number)
		return;

	in.read(reinterpret_cast<char *>(&R), sizeof(uint64_t));
	in.read(reinterpret_cast<char *>(&range), sizeof(uint64_t));

	// fix endian-ness of parameters
	if (is_little_endian){
		R = __builtin_bswap64(R); 
		range = __builtin_bswap64(range);
	}

	_R = R; _range = range; 
	uint32_t* recovered_sketch = new uint32_t[_R*_range]; 

	// now read the sketch into recovered_sketch
	in.read(reinterpret_cast <char *>(recovered_sketch), _R*_range*sizeof(uint32_t));

	delete[] _sketch;
	_sketch = new unsigned int[_R *_range];

	for (size_t r = 0; r < _R; r++){
		for (size_t i = 0; i < _range; i++){
			uint32_t value = recovered_sketch[r*_range + i];
			// 3. Depending on (1), bitflip so that it is big-endian (network order)
			if (is_little_endian)
				value = __builtin_bswap32(value);
			// 4. Save to our sketch 
			_sketch[r*_range + i] = value;
		}
	}
	delete[] recovered_sketch; 
}

void RACE::pprint(std::ostream& out, int width, bool format){
	for (size_t r = 0; r < _R; r++){
		if (format)
			out << std::string((width+1)*_range + 1, '-') << std::endl; 
		for (size_t i = 0; i < _range; i++)
			out << '|' << std::setw(width) << _sketch[r*_range + i];
		out << '|' << std::endl; 
	}
	if (format)
		out << std::string((width+1)*_range + 1, '-') << std::endl; 
}

