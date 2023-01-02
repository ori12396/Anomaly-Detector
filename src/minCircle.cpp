#include "minCircle.h"
#include <assert.h>
#include <algorithm>
#include <cmath>

// Returns the distance between 2 points
float dis(const Point& a, const Point& b) {
    return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

// checks if point is inside or on the given circle
bool isInside(const Circle& c, const Point& p) {
    return dis(c.center, p) <= c.radius;
}

// returns a center point of a circle defined by 3 points
Point getCircleCenter(float ax, float ay, float bx, float by) {
    float B = ax * ax + ay * ay;
    float C = bx * bx + by * by;
    float D = ax * by - ay * bx;
    return { (by * B - ay * C) / (2 * D),(ax * C - bx * B) / (2 * D) };
}

// creates a min circle from 3 points
Circle circle_from(const Point& A, const Point& B, const Point& C) {
    Point I = getCircleCenter(B.x - A.x, B.y - A.y,C.x - A.x, C.y - A.y);
    I.x += A.x;
    I.y += A.y;
    return { I, dis(I, A) };
}

// creates a min circle from 2 points
Circle circle_from(const Point& A, const Point& B) {
    Point C = { (float) ((A.x + B.x) / 2.0), (float)((A.y + B.y) / 2.0) };
    return { C, (float)(dis(A, B) / 2.0) };
}

// checks if given circle encloses given array of points
bool isValidCircle(const Circle& c, const vector<Point>& P) {
    for (const Point& p : P)
        if (!isInside(c, p))
            return false;
    return true;
}

// returns the min enclosing circle for |N|<=3
Circle minCircleTrivial(vector<Point>& P) {
    if (P.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (P.size() == 1) {
        return { P[0], 0 };
    }
    else if (P.size() == 2) {
        return circle_from(P[0], P[1]);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle_from(P[i], P[j]);
            if (isValidCircle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
}

// Returns the MEC using Welzl's algorithm
Circle welzl_helper(vector<Point>& P, vector<Point> R, size_t size) {
    if (size == 0 || R.size() == 3) {
        return minCircleTrivial(R);
    }
    int i = rand() % size;
    Point p = P[i];
    swap(P[i], P[size - 1]);
    Circle d = welzl_helper(P, R, size - 1);
    if (isInside(d, p)) {
        return d;
    }
    R.push_back(p);
    return welzl_helper(P, R, size - 1);
}

Circle welzl(const vector<Point>& P, size_t size) {
    vector<Point> P_copy = P;
    random_shuffle(P_copy.begin(), P_copy.end());
    return welzl_helper(P_copy, {}, size);
}

std::vector<Point> createVectorPoint(Point** points, size_t size) {
    std::vector<Point> vec;
    vec.reserve(size);
    for (int i = 0; i < size; i++) {
        vec.push_back(*points[i]);
    }
    return vec;
}

Circle findMinCircle(Point** points, size_t size) {
    std::vector<Point> P = createVectorPoint(points, size);
    return welzl(P, size);
}


