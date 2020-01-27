#include "catch.h"
#include "SKA.h"
#include "util.h"
#include "io.h"

#include <math.h>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>


TEST_CASE("SKA::samples", "[SKA]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[SKA]    samples" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dataset_size = 2500; 

	// initialize a grid dataset
	std::vector<std::vector<double> > dataset;
	dataset.reserve(dataset_size); 

	for (size_t i = 0; i < sqrt(dataset_size); i++){
		for (size_t j = 0; j < sqrt(dataset_size); j++){
			std::vector<double> v = {(double)i,(double)j};
			dataset.push_back(v);
		}
	}

	SKA ska; 
	std::vector<size_t> sketch_indices = ska.samples(dataset,7, 1); 
	
	for (size_t n = 0; n < sketch_indices.size(); n++)
		std::cout << std::setw(5) << sketch_indices[n] << ": (" << dataset[sketch_indices[n]][0] << ',' << dataset[sketch_indices[n]][1] << ')' << std::endl; 

}

TEST_CASE("SKA::weights", "[SKA]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[SKA]    weights" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	size_t dataset_size = 2500; 
	size_t sketch_size = 7;

	// initialize a grid dataset
	std::vector<std::vector<double> > dataset;
	dataset.reserve(dataset_size); 

	for (size_t i = 0; i < sqrt(dataset_size); i++){
		for (size_t j = 0; j < sqrt(dataset_size); j++){
			std::vector<double> v = {(double)i,(double)j}; 
			dataset.push_back(v);
		}
	}

	SKA ska; 
	std::vector<size_t> sketch_indices = ska.samples(dataset,sketch_size,1); 

	double* K = new double[sketch_size*sketch_size]();
	double* kappa = new double[sketch_size]();

	for(size_t i = 0; i < sketch_size; i++){
		for(size_t j = 0; j < sketch_size; j++){
			// K[i*sketch_size + j] = K(i,j)
			K[i*sketch_size + j] = cauchy(L2Distance(dataset[sketch_indices[i]], dataset[sketch_indices[j]]), 10.0); 
		}
	}

	for(size_t i = 0; i < sketch_size; i++){
		kappa[i] = 0; 
		for(size_t n = 0; n < dataset.size(); n++){
			kappa[i] += cauchy(L2Distance(dataset[sketch_indices[i]], dataset[n]), 10.0); 
		}
		kappa[i] = kappa[i] / dataset_size; 
	}


	for(size_t i = 0; i < sketch_size; i++){
		for(size_t j = 0; j < sketch_size; j++){
			std::cout<<K[i*sketch_size + j]<<','; 
		}
		std::cout<<std::endl; 
	}


	std::cout<<std::endl; 
	for(size_t i = 0; i < sketch_size; i++){
		std::cout<<kappa[i]<<','; 
	}
	std::cout<<std::endl; 


	std::vector<double> weights = ska.weights(K, kappa, sketch_size);

	for(size_t i = 0; i < sketch_size; i++){
		std::cout<<weights[i]<<','; 
	}
	std::cout<<std::endl; 

	
}


TEST_CASE("SKA::KDE", "[SKA]"){

	std::cout << "\033[1;34m"<< std::string(79, '=') << "\033[0m" << std::endl; 
	std::cout << "[SKA]    KDE" <<std::endl;
	std::cout << "\033[1;34m" << std::string(79, '=') << "\033[0m"<<std::endl<<std::endl; 

	std::string d = "3.0605,1.6513\n-4.5312,4.6874\n4.6565,5.0285\n-5.4441,4.8049\n5.9768,6.0619\n2.8248,-0.4427\n-0.0142,1.4722\n4.7263,2.0493\n5.4444,4.7132\n4.7631,4.3566\n-5.2412,4.8483\n-4.7605,5.1925\n5.3409,5.7927\n-5.2526,5.6972\n0.5831,0.7945\n5.3335,5.3604\n4.3929,5.2380\n-5.5798,5.1580\n-4.7839,4.0773\n-4.5873,4.5140\n1.8376,0.5539\n-5.9358,5.0252\n5.2004,3.5420\n-4.6796,4.8708\n2.2616,0.3042\n-5.3361,4.5073\n5.2158,5.6924\n0.7942,0.0735\n1.5778,0.0349\n0.1356,0.6311\n5.8043,4.6619\n-4.7998,4.3590\n2.1256,2.4235\n5.0555,5.9394\n-4.6303,5.1649\n5.6019,4.2463\n5.5089,6.0567\n-5.0754,4.9356\n3.8228,5.8345\n0.4399,0.6260\n-4.5980,5.4640\n1.1889,0.2161\n-5.1876,5.0723\n2.2342,2.1433\n5.7146,6.7285\n-4.8057,5.5186\n4.7664,5.4400\n-5.1252,5.2473\n5.7325,3.8289\n4.8551,5.3594\n6.0600,5.3089\n-5.9110,4.9578\n2.2654,0.9559\n4.6516,5.2297\n-6.2654,4.4764\n5.7401,5.4559\n6.1698,4.0780\n2.2617,0.4997\n-4.4509,5.0516\n5.0244,5.2705\n3.8602,0.8486\n0.4549,1.6178\n1.8407,0.4167\n-4.8323,5.7175\n-4.3658,5.3425\n-4.7640,5.6393\n-5.1254,5.2612\n3.0333,1.2665\n5.9974,6.0867\n3.1330,7.1782\n5.7455,5.9416\n-2.9992,4.1188\n4.0318,5.6611\n5.3776,5.0405\n2.7770,-0.5703\n2.2589,-0.5457\n3.0790,2.5258\n4.9537,6.0000\n2.3637,-0.6030\n0.7324,2.8763\n5.8835,4.4717\n5.0926,4.7251\n4.5296,6.0026\n2.7408,2.5859\n-5.1940,5.4457\n5.6888,6.4353\n3.9296,-0.1342\n1.8666,1.2262\n-5.0889,4.6101\n-5.4294,5.2319\n3.8077,1.7238\n0.9895,-0.5212\n1.6730,3.5888\n2.4397,-0.6213\n-4.6521,5.1246\n-3.8562,4.7785\n-3.6942,5.4054\n3.1702,0.0196\n-5.3985,5.0989\n-4.5506,5.7269"; 
	std::string q = "-4,5\n0,0\n-2,1\n6,0\n6,6\n0,7"; 
	
	std::stringstream data; 
	data << d; 
	std::stringstream queries; 
	queries << q; 

	size_t n_samples = 5; 
	double sigma = 4.0; 


    // input buffer
    std::vector<double> vec;

	std::vector<std::vector<double> > dataset;
	dataset.reserve(100);

    size_t DataID = 0; 
    do{
    	VectorFeatures(data,vec);
    	if (vec.size() == 0)continue; 
    	dataset.push_back(vec); 
        DataID++;
    }
    while(data);


    size_t dataset_size = dataset.size(); 

	SKA ska; 
	std::vector<size_t> samples = ska.samples(dataset,n_samples,1); 

	double* K = new double[n_samples*n_samples]();
	double* kappa = new double[n_samples]();

	for(size_t i = 0; i < n_samples; i++){
		for(size_t j = 0; j < n_samples; j++){
			K[i*n_samples + j] = cauchy(L2Distance(dataset[samples[i]], dataset[samples[j]]), sigma);
		}
	}
	for(size_t i = 0; i < n_samples; i++){
		kappa[i] = 0;
		for(size_t n = 0; n < dataset.size(); n++){
			kappa[i] += cauchy(L2Distance(dataset[samples[i]], dataset[n]), sigma); 
		}
		kappa[i] = kappa[i] / dataset_size; 
	}
	std::vector<double> weights = ska.weights(K, kappa, n_samples);

	std::vector<double> estimates; 
	std::vector<double> gtruth; 

    do{
        VectorFeatures(queries,vec);
		if (vec.size() == 0)continue; 
        double khat = 0; // kde estimate
        for (size_t i = 0; i < samples.size(); i++){
        	khat += cauchy(L2Distance(vec, dataset[samples[i]]), sigma)*weights[i];
        }
        estimates.push_back(khat);
        double k = 0;
        for(size_t n = 0; n < dataset.size(); n++){
        	k += cauchy(L2Distance(vec, dataset[n]), sigma);
        }
        gtruth.push_back(k/dataset.size()); 
    }
    while(queries);

    std::cout<<"Selected samples: "<<std::endl; 
    for(auto& v : samples){
    	std::cout<<"\t("<<dataset[v][0]<<","<<dataset[v][1]<<")"<<std::endl; 
    }

    std::cout<<"estimate | KDE"<<std::endl; 
    for (size_t i = 0; i < estimates.size(); i++){
    	std::cout<<estimates[i]<<'|'<<gtruth[i]<<std::endl; 
    }


}



