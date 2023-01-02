/*
 * Author: Edward Gutman 311432082, Ori Zohar 207632795
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
    Circle minCirlce;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
    float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    virtual void learnNormalHelper(const TimeSeries& ts, string& f1, string& f2,
                                   float correlation, Point **points, int size);
    virtual void detectHelper(std::vector<AnomalyReport>& reports, Point& p, correlatedFeatures& cf, int time);

    vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    float getThreshold(){
        return threshold;
    }

    void setThreshold(float t){
        threshold = t;
    }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
