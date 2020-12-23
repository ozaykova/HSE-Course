#pragma once
#include "ellipse.h"

class Circle : public Ellipse {
public:
    Circle(const Point& c, double r);
    double radius();
};