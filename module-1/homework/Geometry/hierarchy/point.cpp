#include "point.h"
#include <cmath>

Point::Point(double a, double b) : x(a), y(b) {}

double Point::dist(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y-b.y));
}

bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}

bool operator==(const Point& a, const Point& b) {
    return (std::abs(a.x - b.x) < EPS) && (std::abs(a.y - b.y) < EPS);
}

bool operator<(const Point& a, const Point& b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}