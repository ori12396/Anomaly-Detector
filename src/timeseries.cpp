/*
 * Author: Edward Gutman 311432082, Ori Zohar 207632795
 */

#include "timeseries.h"

// create a TimeSeries from a given file.
TimeSeries::TimeSeries(const char* CSVfileName) {
    // open the file
    std::ifstream file(CSVfileName);
    std::string line, cell;
    bool init = true;

    // iterate on each line
    while (std::getline(file, line)) {
        // open each line as string stream
        std::stringstream ss(line);

        // first line is the features line
        if (init) {

            // split the line by ','
            while(std::getline(ss,cell, ',')) {
                // add to the feature's vector
                features.push_back(cell);
                // create the floats vector
                std::vector<float> numbers;
                data.push_back(numbers);
            }
            init = false;
            continue;
        }

        // add the data to the float vectors.
        auto p = data.begin();
        while(std::getline(ss,cell, ',')) {
            p++->push_back(std::stof(cell));
        }
    }
}

// return vector of the attributes
std::vector<std::string> TimeSeries::getAttributes() const {
    return features;
}

// return a column of data of given attribute index
std::vector<float> TimeSeries::getVector(int index) const{
    return data[index];
}

// return the data of given feature and timestep
float TimeSeries::getValue(const std::string& feature, int timestep) const{
    auto it = find(this->features.begin(), this->features.end(), feature);

    // If element was found
    if (it != this->features.end()) {
        // calculating the index of the element
        int index = it - this->features.begin();
        return data[index][timestep];
    }
    else {
        // If the element is not present in the vector
        std::cout << "element not found" << std::endl;
        return 0;
    }
}

// return the number of features
unsigned long TimeSeries::featureSize() const {
    return this->features.size();
}

// return the number of timesteps
unsigned long TimeSeries::dataSize() const {
    return data[0].size();
}

// return a data array of the given feature index
float *TimeSeries::getArray(int index) const {
    return (float *) &data[index][0];
}


