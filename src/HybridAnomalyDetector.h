

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();


    void learnNormalHelper(const TimeSeries &ts, string &f1, string &f2,
                           float correlation, Point **points, int size) override;

    void detectHelper(std::vector<AnomalyReport>& reports, Point& p, correlatedFeatures& cf, int time) override;

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
