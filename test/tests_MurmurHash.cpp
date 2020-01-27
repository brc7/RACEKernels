#include "catch.h"

#include "MurmurHash.h"
#include <iostream>

TEST_CASE("MurmurHash test on integer","[MurmurHash]"){
	int input = 10; 
	unsigned int result = MurmurHash (&input, sizeof(int), 0); 
	CHECK(result == 616745908); 
}

TEST_CASE("MurmurHash test on dynamic int array","[MurmurHash]"){
	int N = 24; 
	int *input = new int[N]; 
	for (int i = 0; i < N; i++)
		input[i] = i; 

	SECTION("Single array"){
		unsigned int result = MurmurHash(input, sizeof(int)*N, 0); 
		CHECK(result == 1278141134); 
	}
	SECTION("Segmented array"){
		unsigned int result = MurmurHash(input, sizeof(int)*8, 0);
		CHECK(result == 623569259); 
		result = MurmurHash(input + 8, sizeof(int)*8, 0);
		CHECK(result == 450903091); 
		result = MurmurHash(input + 16, sizeof(int)*8, 0);
		CHECK(result == 334506121); 
	}
}
