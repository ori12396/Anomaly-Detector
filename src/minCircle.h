
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c, float r): center(c), radius(r){}
};
// --------------------------------------

bool isInside(const Circle& c, const Point& p);
Circle findMinCircle(Point** points,size_t size);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
