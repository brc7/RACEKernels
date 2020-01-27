#include "catch.h"
#include "util.h"

#include <math.h>
#include <iomanip>
#include <vector>
#include <unordered_map>



TEST_CASE("util::L2Distance", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    L2Distance" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::vector<double> x = {1,1,1,1};
	std::vector<double> y = {2,0,2,0};

	std::cout<<L2Distance(x,y)<<std::endl; 
}

TEST_CASE("util::L2Distance (Map-Vector)", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    L2Distance (Map-Vector)" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::unordered_map<size_t, double> x; 
	x[0] = 10; x[1] = -4; x[2] = 1; x[3] = 1; 
	std::vector<double> y = {2,0,2,0};
	std::cout<<L2Distance(y,x)<<std::endl; 
}

TEST_CASE("util::L2Distance (Maps)", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    L2Distance (Maps)" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::unordered_map<size_t, double> x; 
	std::unordered_map<size_t, double> y; 
	x[0] = 10; x[1] = -4; x[2] = 1; x[3] = 1; 
	y[0] = 2; y[2] = 2; 
	std::cout<<L2Distance(x,y)<<std::endl; 
}


TEST_CASE("util::lsh_kernel_L1", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    lsh_kernel_L1" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::vector<double> x = {1,1,1,1};
	std::vector<double> y = {2,0,2,0};
	std::cout<<lsh_kernel_L1(L1Distance(x,y),0.5)<<std::endl;
}

TEST_CASE("util::lsh_kernel_L2", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    lsh_kernel_L2" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::vector<double> x = {1,1,1,1};
	std::vector<double> y = {2,0,2,0};
	std::cout<<lsh_kernel_L2(L2Distance(x,y),0.5)<<std::endl;
}

TEST_CASE("util::InnerProduct", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    InnerProduct" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::vector<double> x = {1,1,-1,1};
	std::vector<double> y = {2,0,1,0};

	std::cout<<InnerProduct(x,y)<<std::endl; 
}

TEST_CASE("util::InnerProduct (Map-Vector)", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    InnerProduct (Map-Vector)" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::unordered_map<size_t, double> x; 
	x[0] = 10; x[1] = -4; x[2] = -1; x[3] = 1; 
	std::vector<double> y = {2,0,2,0};
	std::cout<<InnerProduct(y,x)<<std::endl; 
}

TEST_CASE("util::InnerProduct (Maps)", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    InnerProduct (Maps)" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::unordered_map<size_t, double> x; 
	std::unordered_map<size_t, double> y; 
	x[0] = 10; x[1] = -4; x[2] = -1; x[3] = 1; 
	y[0] = 2; y[2] = 2; 
	std::cout<<InnerProduct(x,y)<<std::endl; 
}



TEST_CASE("util::L2Norm", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    L2Norm" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::vector<double> x = {1,1,1,1};

	std::cout<<L2Norm(x)<<std::endl; 
}

TEST_CASE("util::L2Norm (Map)", "[util]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[util]    L2Norm (Map)" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::unordered_map<size_t, double> x; 
	x[0] = 10; x[1] = -4; x[2] = 1; x[3] = 1; 
	std::cout<<L2Norm(x)<<std::endl; 
}

