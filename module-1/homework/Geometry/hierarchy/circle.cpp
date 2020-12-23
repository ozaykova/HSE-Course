#include "circle.h"

Circle::Circle(const Point& c, double r) : Ellipse(c, c, r) { }

double Circle::radius() {
    return dist;
}
