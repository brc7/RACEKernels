#include "util.h"


double normcdf(double value){
   return 0.5 * erfc(-value * M_SQRT1_2);
}

double lsh_kernel_L1(double c, double sigma){
	if (c == 0.0){
		return 1.0; 
	} else {
		return (2.0/PI) * std::atan(sigma/c) - ( c / (PI*sigma) ) * std::log(1.0 + sigma*sigma/(c*c)); 
		// 2*np.arctan(w/c)/np.pi - c/(np.pi * w) * np.log(1 + (w/c)**2)
	}
}
double lsh_kernel_L2(double c, double sigma){
	if (c == 0.0){
		return 1.0; 
	} else {
		// 1 - 2*norm.cdf(-w/c) - 2.0/(math.sqrt(2*math.pi)*(w/c)) * (1 - math.exp(-0.5*(w**2)/(c**2)) 
		return 1.0 - 2.0*normcdf(-sigma/c) - 2.0*c/(SQRT_2PI*sigma) * (1 - std::exp(-0.5*sigma*sigma/(c*c))); 
	}
}
double cauchy(double c, double sigma){
	return 1.0/(1.0 + c*c/(sigma*sigma) ); 
}
double inverse_multiquadric(double c, double sigma){
	return sigma/std::sqrt(c*c + sigma*sigma); 
}
double rational_quadratic(double c, double sigma){
	return 1.0 - (c*c)/(c*c + sigma); 
}
double student_t(double c, int sigma){
	return 1.0/(1.0 + std::pow(c, sigma) ); 
}

double angular_kernel(double c, int sigma){
	// defined as (1 - theta/pi) optionally raised to the sigma power
	return std::pow(1.0 - c/PI,sigma); 
}

void rehash(int* input_hashes, int* output_hashes, int nhashes, int values_per_set){
    #pragma omp parallel for 
    for (size_t i = 0; i < nhashes; i++){
       output_hashes[i] = MurmurHash(input_hashes + values_per_set*i, sizeof(int)*values_per_set, 42);
    }
}

double L2Distance(const std::vector<double>& x, const std::vector<double>& y){
	// returns L2 distance 
	size_t dimension = x.size(); 
	double distance = 0; 
	for(size_t i = 0; i < dimension; i++)
		distance += (x[i]-y[i])*(x[i] - y[i]);
	distance = std::sqrt(distance); 
	return distance; 
}

double L2Distance(const std::unordered_map<size_t, double>& y, const std::vector<double>& x){
	return L2Distance(x,y);
}

double L2Distance(const std::vector<double>& x, const std::unordered_map<size_t, double>& y){
	double distance = 0; 
	for (const auto& value : y){
		distance += (x[value.first] - value.second)*(x[value.first] - value.second); 
	}
	distance = std::sqrt(distance); 
	return distance; 
}

double L2Distance(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t, double>& y){
	double distance = 0;
	for (const auto& value : x){
		auto iterator = y.find(value.first);
		if (iterator != y.end()){
			distance += (value.second - iterator->second)*(value.second - iterator->second); 
		} else {
			distance += value.second*value.second; 
		}
	}
	for (const auto& value : y){
		auto iterator = x.find(value.first); 
		if (iterator == x.end()){
			distance += value.second*value.second; 
		}
	}
	distance = std::sqrt(distance);
	return distance;
}

double L2Norm(const std::vector<double>& x){
	// returns L2 norm
	size_t dimension = x.size(); 
	double norm = 0; 
	for(size_t i = 0; i < dimension; i++)
		norm += x[i]*x[i]; 
	norm = std::sqrt(norm); 
	return norm; 
}

double L2Norm(const std::unordered_map<size_t, double>& x){
	// returns L2 norm
	double norm = 0; 
	for (const auto& value : x){
		norm += value.second * value.second; 
	}
	norm = std::sqrt(norm); 
	return norm;
}

double L1Distance(const std::vector<double>& x, const std::vector<double>& y){
	// returns L1 distance 
	size_t dimension = x.size(); 
	double distance = 0; 
	for(size_t i = 0; i < dimension; i++)
		distance += std::abs(x[i] - y[i]); 
	return distance; 
}

double L1Distance(const std::vector<double>& x, const std::unordered_map<size_t, double>& y){
	double distance = 0; 
	for (const auto& value : y){
		distance += std::abs(x[value.first] - value.second); 
	}
 	return distance; 
}

double L1Distance(const std::unordered_map<size_t, double>& y, const std::vector<double>& x){
	return L1Distance(x,y);
}

double L1Distance(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t, double>& y){
	double distance = 0;
	for (const auto& value : x){
		auto iterator = y.find(value.first);
		if (iterator != y.end()){
			distance += std::abs(value.second - iterator->second); 
		} else {
			distance += std::abs(value.second); 
		}
	}
	for (const auto& value : y){
		auto iterator = x.find(value.first); 
		if (iterator == x.end()){
			distance += std::abs(value.second); 
		}
	}
	return distance;
}

double L1Norm(const std::vector<double>& x){
	// returns L1 norm
	size_t dimension = x.size(); 
	double norm = 0; 
	for(size_t i = 0; i < dimension; i++)
		norm += std::abs(x[i]); 
	return norm;
}

double L1Norm(const std::unordered_map<size_t, double>& x){
	// returns L1 norm
	double norm = 0; 
	for (const auto& value : x){
		norm += std::abs(value.second); 
	}
	return norm;
}

double InnerProduct(const std::vector<double>& x, const std::vector<double>& y){
	double result = 0; 
	size_t dimension = x.size(); 
	for(size_t i = 0; i < dimension; i++)
		result += x[i]*y[i]; 
	return result; 
}


double InnerProduct(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t,double>& y){
	double result = 0; 
	for (const auto& value : x){
		auto iterator = y.find(value.first); 
		if (iterator != y.end()){
			result += value.second * (iterator->second); 
		}
	}
	return result; 
}

double InnerProduct(const std::unordered_map<size_t,double>& x, const std::vector<double>& y){
	double result = 0; 
	for (const auto& value : x){
		result += value.second * y[value.first]; 
	}
	return result; 
}

double InnerProduct(const std::vector<double>& y, const std::unordered_map<size_t,double> & x){
	return InnerProduct(x,y); 
}
