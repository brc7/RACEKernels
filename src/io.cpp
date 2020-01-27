#include "io.h" 

bool VectorFeatures(std::istream& in, std::vector<double>& vec, size_t skip) {

    std::string line; 
    std::getline(in, line); 
    std::stringstream ss(line); 
    vec.clear(); 
    double element;

    size_t i = 0; 
    bool ret = false; 
    while (ss >> element)
    {
        ret = true; 
        if (i >= skip){
        vec.push_back(element); 
        if (ss.peek() == ',')
            ss.ignore(); 
        }
        ++i; 
    }
    return ret; 
    // courtesy of: https://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring
}

bool VectorFeatures(std::istream& in, std::vector<double>& vec){
    return VectorFeatures(in, vec, 0); 
}

void WriteVectorCSV(std::ostream& out, const std::vector<double>& values){
    for(size_t i = 0; i < values.size(); i++){
        out<<values[i]; 
        if (i != values.size()-1){
            out<<','; 
        }
    }
    out<<std::endl; 
}

/* DEPRECATED */
void WriteCSVResults(std::ostream& out, size_t sketch_size, double preprocessing_time, double query_time, std::vector<double>& estimates){
    // Writes a CSV file row 
    // Assumes out starts at the start of the row (does not write leading newline)
    // Writes the following: 
    // [sketch_size in cells], [preprocessing time in seconds], [total querying time in seconds], KDE1, KDE2, KDE3, KDE4, (all doubles)
    out << sketch_size; 
    out << ',' << preprocessing_time;
    out << ',' << query_time;
    for(size_t i = 0; i < estimates.size(); i++)
        out << ',' << estimates[i];
    out<<std::endl; 
}


bool SparseVectorFeatures(std::istream& in, std::vector<double>& vec, size_t dimensionality, size_t skip){
    // skip is the number of header elements to skip
    std::string line; 
    std::getline(in, line); 
    std::replace( line.begin(), line.end(), ':', ' '); 
    std::stringstream ss(line); 
    vec.resize(dimensionality); // sets to all zeros 
    
    double element;
    size_t ID; 

    for (size_t i = 0; i < skip; ++i){
        ss.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    }

    bool ret = false; 
    while (ss >> ID){
        ret = true; 
        ss >> element; 
        vec[ID] = element; 
    }
    return ret; 
}

bool SparseVectorFeatures(std::istream& in, std::vector<double>& vec, size_t dimensionality){
    return SparseVectorFeatures(in, vec, dimensionality, 0); 
}

bool SparseMapFeatures(std::istream& in, std::unordered_map<size_t,double>& map, size_t skip){
    // skip is the number of header elements to skip
    std::string line; 
    std::getline(in, line); 
    std::replace( line.begin(), line.end(), ':', ' '); 
    std::stringstream ss(line); 

    map.clear(); 
    
    double element;
    size_t ID; 

    for (size_t i = 0; i < skip; ++i){
        ss.ignore(std::numeric_limits<std::streamsize>::max(),' ');
    }

    bool ret = false; 
    while (ss >> ID){
        ret = true; 
        ss >> element; 
        map[ID] = element; 
    }
    return ret; 
}

bool SparseMapFeatures(std::istream& in, std::unordered_map<size_t,double>& map){
    return SparseMapFeatures(in, map, 0); 
}

