/*
 * Author: Edward Gutman 311432082, Ori Zohar 207632795
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

class TimeSeries{
    std::vector<std::string> features;
    std::vector<std::vector<float>> data;

public:

	TimeSeries(const char* CSVfileName);
    std::vector<std::string> getAttributes() const;
    std::vector<float> getVector(int index) const;
    float* getArray(int index) const;
    float getValue(const std::string& feature, int timestep) const;
    unsigned long featureSize() const;
    unsigned long dataSize() const;

};



#endif /* TIMESERIES_H_ */
