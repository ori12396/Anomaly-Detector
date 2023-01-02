
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() = default;
HybridAnomalyDetector::~HybridAnomalyDetector() = default;

void HybridAnomalyDetector::learnNormalHelper(const TimeSeries &ts, string& f1, string& f2,
                                              float correlation, Point **points, int size) {
    SimpleAnomalyDetector::learnNormalHelper(ts, f1, f2, correlation, points, size);
    if (abs(correlation) >= 0.5 && abs(correlation) < threshold) {
        Circle minCircle = findMinCircle(points, size);
        minCircle.radius = minCircle.radius * (float)1.1;
        correlatedFeatures newCorrelation = {f1, f2, correlation, Line(),
                                             minCircle, 0};
        // add the correlated features pair to cf
        this->cf.push_back(newCorrelation);
    }
}

void HybridAnomalyDetector::detectHelper(std::vector<AnomalyReport>& reports, Point &p, correlatedFeatures &cf, int time) {
    SimpleAnomalyDetector::detectHelper(reports, p, cf, time);
    if (abs(cf.corrlation) >= 0.5 && abs(cf.corrlation) < threshold && !isInside(cf.minCirlce, p)) {
        AnomalyReport report = AnomalyReport(cf.feature1 + "-" + cf.feature2,
                                             time+1);
        reports.push_back(report);
    }
}

