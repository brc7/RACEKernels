#include "catch.h"
#include "io.h"
#include <string>
#include <sstream>




TEST_CASE("VectorFeatures correctly encodes CSV string","[io]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[io]    VectorFeatures" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::string input = "1,2,507,13312740\n";
	std::istringstream ss_in(input); 
	double correct_values[4] = {1,2,507,13312740}; 

	std::vector<double> vec; 

	VectorFeatures(ss_in, vec); 

	REQUIRE(vec.size() == 4);
	for (int i = 0; i < 4; i++)
	{
		CHECK(vec[i] == correct_values[i]);
	}
}


TEST_CASE("SparseVectorFeatures correctly encodes whitespace-separated string","[io]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[io]    SparseVectorFeatures" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::string input = "1 2.01 5 100.42 82 391.0 \n";
	std::istringstream ss_in(input); 

	std::vector<double> v; 
	SparseVectorFeatures(ss_in, v, 83);
	for (size_t i = 0; i < v.size(); i++){
		if (v[i] != 0)
			std::cout<<"v["<<i<<"] : "<<v[i]<<std::endl; 
	}
}


TEST_CASE("SparseVectorFeatures correctly skips with whitespace-separated string","[io]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[io]    SparseVectorFeatures with skip" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::string input = "+1 1 2.01 5 100.42 82 391.0 \n";
	std::istringstream ss_in(input); 

	std::vector<double> v; 
	SparseVectorFeatures(ss_in, v, 83, 1);
	for (size_t i = 0; i < v.size(); i++){
		if (v[i] != 0)
			std::cout<<"v["<<i<<"] : "<<v[i]<<std::endl; 
	}
}

TEST_CASE("SparseVectorFeatures correctly works with libsvm format","[io]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[io]    SparseVectorFeatures with libsvm" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::string input = "-1 1:2.01 5:100.42 82:391.0 \n";
	std::istringstream ss_in(input); 

	std::vector<double> v; 
	SparseVectorFeatures(ss_in, v, 83, 1);
	for (size_t i = 0; i < v.size(); i++){
		if (v[i] != 0)
			std::cout<<"v["<<i<<"] : "<<v[i]<<std::endl; 
	}
}
