#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

// #include <math.h>
#include <cmath>

#include "MurmurHash.h"
#define PI 3.14159265358979323846
#define SQRT_2PI 2.5066282746310002

// mathematic and hash utilities, collected in one place
// for instance, we define kernels in here
// c is the L2 distance, except for lsh_kernel_L1 in which case it is the L1 distance

double cauchy(double c, double sigma); 
double student_t(double c, int power); 
double inverse_multiquadric(double c, double sigma); 
double rational_quadratic(double c, double sigma); 
double lsh_kernel_L1(double c, double sigma); 
double lsh_kernel_L2(double c, double sigma); 
double angular_kernel(double c, int sigma); 

double L2Distance(const std::vector<double>& x, const std::vector<double>& y); 
double L2Distance(const std::vector<double>& x, const std::unordered_map<size_t, double>& y); 
double L2Distance(const std::unordered_map<size_t, double>& y, const std::vector<double>& x); 
double L2Distance(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t, double>& y); 

double L1Distance(const std::vector<double>& x, const std::vector<double>& y); 
double L1Distance(const std::vector<double>& x, const std::unordered_map<size_t, double>& y); 
double L1Distance(const std::unordered_map<size_t, double>& y, const std::vector<double>& x); 
double L1Distance(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t, double>& y); 

double L2Norm(const std::vector<double>& x); 
double L2Norm(const std::unordered_map<size_t,double>& x); 

double L1Norm(const std::vector<double>& x); 
double L1Norm(const std::unordered_map<size_t,double>& x); 

double InnerProduct(const std::vector<double>& x, const std::vector<double>& y); 
double InnerProduct(const std::unordered_map<size_t,double>& x, const std::unordered_map<size_t,double>& y); 
double InnerProduct(const std::unordered_map<size_t,double>& x, const std::vector<double>& y); 
double InnerProduct(const std::vector<double>& y, const std::unordered_map<size_t,double> & x); 

void rehash(int* input_hashes, int* output_hashes, int nhashes, int values_per_set); 

