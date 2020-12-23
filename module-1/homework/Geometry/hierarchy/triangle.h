#pragma once
#include "polygon.h"
#include "circle.h"
#include <vector>

class Triangle : public Polygon {
public:
    Triangle(const Point& a, const Point& b, const Point& c) : Polygon(std::vector<Point>{a, b, c}) {}
    Circle circumscribedCircle();
    Circle inscribedCircle();
};