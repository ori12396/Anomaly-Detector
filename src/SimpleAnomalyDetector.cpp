/*
 * Author: Edward Gutman 311432082, Ori Zohar 207632795
 */
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

// create an array of points of given two arrays of data
Point** createPointsArray (float *x, float *y, int size) {
    auto **p = new Point*[size];
    for (int i = 0; i < size; i++) {
        auto* h = new Point(*(x + i), *(y + i));
        *(p + i) = h;
    }
    return p;
}

// free a given points array
void releasePointsArray (Point** p, int size) {
    for (int i = 0; i < size; i++) {
        delete *(p + i);
    }
    delete[] p;
}

// find the max threshold
float findThreshold (Line ln, Point** p, int size) {
    float max = 0;
    for (int i = 0; i < size; i++) {
        float distance = dev(**(p + i), ln);
        if (distance > max)
            max = distance;
    }
    return max * (float)1.1;
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // traverse on each feature
    for (int i = 0; i < ts.featureSize(); i++) {
        float max = 0;
        int c = -1;

        // traverse on the rest of the features
        for (int j = i + 1; j < ts.featureSize(); j++) {
            // calculate the pearson
            float p = pearson(ts.getArray(i), ts.getArray(j), ts.dataSize());

            // check if its bigger than the max pearson
            if (std::abs(p) > max) {
                max = p;
                c = j;
            }
        }

        if (c != -1) {
            // create a correlated features pair
            int size = ts.dataSize();
            Point **points = createPointsArray(ts.getArray(i), ts.getArray(c), size);

            learnNormalHelper(ts, ts.getAttributes()[i], ts.getAttributes()[c], max, points, size);

            // free the point array
            releasePointsArray(points, size);
        }
    }
}

void SimpleAnomalyDetector::learnNormalHelper(const TimeSeries &ts, string& f1, string& f2,
                                              float correlation, Point **points, int size) {
    if (abs(correlation) >= threshold) {
        Line ln = linear_reg(points, size);
        float threshold = findThreshold(ln, points, size);
        correlatedFeatures newCorrelation = {f1, f2, correlation, ln,
                                             Circle({0,0}, 0), threshold};
        // add the correlated features pair to cf
        this->cf.push_back(newCorrelation);
    }
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    std::vector<AnomalyReport> reports;
    int size = ts.dataSize();

    // traverse on each timestep line
    for (auto it = cf.begin(); it < cf.end(); it++) {

        // traverse on each correlated feature
         for (int i = 0; i < size; i++) {
            Point p = {ts.getValue(it->feature1, i), ts.getValue(it->feature2, i)};
            detectHelper(reports, p, *it, i);
        }
    }
    return reports;
}

// check if there is an anomaly and add it
void SimpleAnomalyDetector::detectHelper(std::vector<AnomalyReport>& reports, Point &p,
                                         correlatedFeatures &correlatedFeatures, int time) {
    if (abs(correlatedFeatures.corrlation) >= threshold && dev(p, correlatedFeatures.lin_reg) > correlatedFeatures.threshold) {
        AnomalyReport report = AnomalyReport(correlatedFeatures.feature1 + "-" + correlatedFeatures.feature2,
                                             time+1);
        reports.push_back(report);
    }
}

