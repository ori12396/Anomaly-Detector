/*
 * animaly_detection_util.cpp
 *
 * Author: Edward Gutman 311432082, Ori Zohar 207632795
 */

#include <cmath>
#include "anomaly_detection_util.h"

// returns the average of X
float avg(float* x, int size){
    float sum = 0;
	for (int i = 0; i < size; i++) {
        sum += *(x + i);
    }
    return (sum/size);
}

// returns the variance of X and Y
float var(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += powf(*(x + i), 2);
    }
    return (sum/size - powf(avg(x,size), 2));
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float sum = 0;
    float px, py;
    for (int i = 0; i < size; i++) {
        px = *(x + i);
        py = *(y + i);
        sum += px * py;
    }
    return (sum/size - avg(x, size) * avg(y, size));
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return cov(x, y, size) / sqrt(var(x, size) * var(y, size));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float a, b;
    float x[size];
    float y[size];

    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }

    a = cov(x, y, size) / var(x, size);
    b = (avg(y, size) - (a * avg(x, size)));
	return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	return dev(p, linear_reg(points, size));
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    return std::abs(p.y - l.f(p.x));
}