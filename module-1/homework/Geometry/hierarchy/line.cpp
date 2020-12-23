#include "line.h"

Line::Line(Point a, Point b) : x(a), y(b) {}

bool operator!=(const Line& a, const Line& b) {
    return !(a == b);
}

Vector Line::getDirectionVector() const {
    return Vector(x, y);
}

bool operator==(const Line& a, const Line& b) {
    Vector l1(a.x, a.y), l2(b.x, b.y), m(a.x, b.x);
    return Vector::vectorComposition(l1, l2) == 0
        && Vector::vectorComposition(l1, m) == 0
        && Vector::vectorComposition(l2, m) == 0;
}
