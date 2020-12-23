#pragma once
#include "vector.h"

class Line {
public:
    Line(Point a, Point b);

    friend bool operator==(const Line& a, const Line& b);
    friend bool operator!=(const Line& a, const Line& b);

    Vector getDirectionVector() const;

    Point x;
    Point y;
};