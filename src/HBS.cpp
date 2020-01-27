#include "HBS.h"


/* An implementation of the hash based sketch in the Moses 2019 ICML paper 
"Rehashing Kernel Evaluation in High Dimensions"

Steps: 

Given a set number of elements to keep in the sketch

Using M = 5 hash tables

1. Build a hash table
Here we will build a hash table

2. Sub-sample 

Now repeat step 1 - 

*/

void HBS::add(int* hashes, size_t DataID){
	// for each table in tables
	for(size_t i = 0; i < num_tables; ++i){
		int key = hashes[i]; // the hash value of DataID (the key) for table i 
		// add DataID to tables[i]
		tables[i][key].push_back(DataID);
	}
}

std::vector<std::pair<size_t, double> > HBS::samples(size_t sketch_size, size_t dataset_size){
	// accepts the dataset size, which is needed to compute gamma (and some paramters)
	// Also accepts the sketch size, which is the number of elements in the sketch 
	// returns a vector of pairs (DataID, weight)
	// To get the KDE, take the mean of weight * kernel(DataID) over all 
	// the DataIDs that are returned

	// We will need a place to store info about the buckets for each table 
	std::vector<bucket_info> table_info; 
	table_info.reserve(1000); 
	// we will also require a random number generator 
	std::mt19937 generator(seed); 

	// finally we need a place to deposit the IDs that we choose to keep in the sketch 
	std::vector<std::pair<size_t, double> > sketch_IDs;
	
	for(size_t i = 0; i < num_tables; ++i){
		// compute the table_info: the bucket_key, bucket_p, and bucket_weight for each bucket
		
		size_t n_samples = sketch_size / num_tables;
		// n_samples is the number of samples to take from this table

		if (i == num_tables-1) // catch the "overlap" on the last set of sketched points
			n_samples = sketch_size - (sketch_size / num_tables)*i;
		// std::cout<<"N samples from table "<<i<<": "<<n_samples<<std::endl; 

		table_info.clear();
		double gamma = compute_gamma(i, dataset_size);
		// std::cout<<"Gamma = "<<gamma<<std::endl;

		double sum_p = 0; // sum of all sampling probability weights
		for (auto iterator : tables[i]){ 
			// for each bucket in the table,
			// save (key, sampling probability, weight)
			size_t bucket_size = iterator.second.size(); 
			double sampling_probability = pow(bucket_size,gamma);
			sum_p+= sampling_probability;

			double weight = pow(bucket_size,1.0 - gamma);

			bucket_info info = {iterator.first, (int)(bucket_size), sampling_probability, weight}; 
			table_info.push_back(info); 
		}

		// sum_p = sum_p / dataset_size; 
		// std::cout<<"sum_p = "<<sum_p<<std::endl; 

		std::vector<double> sampling_probabilities; 

		for (auto &b : table_info){
			b.bucket_w = b.bucket_w * sum_p / (dataset_size * sketch_size); 
			sampling_probabilities.push_back(b.bucket_p / sum_p); 
			// std::cout<<std::setw(5)<<b.bucket_key<<"|"<<b.bucket_p/sum_p<<' '<<b.bucket_w<<' '<<b.bucket_size<<' '<<std::endl; 
		}

		// now we have a list of probabilities to use
		// We will sample from table i according to the distribution of bucket_p's

		std::discrete_distribution<> bucket_distribution = std::discrete_distribution<>
		(sampling_probabilities.begin(), sampling_probabilities.end());

		for(size_t j = 0; j < n_samples; ++j){
			int idx = bucket_distribution(generator); // index of chosen bucket
			double weight = table_info[idx].bucket_w; 
			int key = table_info[idx].bucket_key; 

            std::uniform_int_distribution<int> uniform(0, table_info[idx].bucket_size - 1);

            // sample a point from the bucket 
            size_t ID = tables[i][key][uniform(generator)]; 
			sketch_IDs.push_back(std::make_pair(ID,weight)); 
		}
	}
	return sketch_IDs; 
}

void HBS::pprint(std::ostream& out, int width, bool format){

	for(size_t i = 0; i < num_tables; ++i){
		out << "------------" << std::endl; 
		out << "| Table "<<std::setw(3)<<i<<"|"<<std::endl; 
		out << "------------" << std::endl; 
		out << "| Key | ID |"<<std::endl; 
		out << "------------" << std::endl; 

		for (auto iterator : tables[i]){
			// for each bucket in the table, print out the contents 
			int key = iterator.first; 
			out << std::setw(5) << key << "| ";
			for(auto const& value: iterator.second) {
				out << std::setw(width) << value<<' '; 
			}
			out<<std::endl; 
		}
	}
}

double HBS::compute_gamma(size_t table_number, size_t dataset_size){

	// I'll be honest 
	// I have no clue what this does 

	size_t max_bucket = 0; 
	size_t num_buckets = 0; 

	for (auto iterator : tables[table_number]){
		// find the size of the largest bucket in table "table number"
		if (iterator.second.size() > max_bucket)
			max_bucket = iterator.second.size(); 
		num_buckets++; 
	}

    double eps = 0.5;
    double tau = 0.001;
    double cst = 0.8908987;  // (1/2)^(1/6)
    double eps_cst = eps * eps / 6;
    double nn = max_bucket / tau / dataset_size;
    double log_delta = 1 / eps_cst * 1.1;
    if (num_buckets <= cst / tau) {
        return 1.0 - log(eps_cst * log_delta) / log(nn);
    }
    else
    	return 1.0; 
}
