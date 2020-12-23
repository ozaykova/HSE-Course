#pragma once
#include <utility>
#include "polygon.h"

class Rectangle : public Polygon {
public:
    Rectangle(const Point& a, const Point& b, double ratio);
    Point center();
    std::pair<Line, Line> diagonals();
};