#include "vector.h"
#include <cmath>


Vector::Vector(double a, double b) : x(a), y(b) {}


Vector::Vector(const Point& p) : x(p.x), y(p.y) {}


Vector::Vector(const Point& a, const Point& b) {
    x = b.x - a.x;
    y = b.y - a.y;
}

double Vector::vectorComposition(const Vector &v1, const Vector &v2){
    return v1.x * v2.y - v1.y * v2.x;
}

double operator*(const Vector& v1, const Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector operator*(const Vector& v, double s) {
    return {v.x * s, v.y * s};
}

Point operator+(const Point& p, const Vector& v) {
    return {p.x + v.x, p.y + v.y};
}

Point operator-(const Point& p, const Vector& v) {
    return {p.x - v.x, p.y - v.y};
}


Vector Vector::getNormal() const {
    return {-y, x};
}

double Vector::getLength() const {
    return sqrt(x * x + y * y);
}

void Vector::normalize() {
    Vector n = *this * (1 / getLength());
    x = n.x;
    y = n.y;
}

Vector Vector::getRotatedVector(double angle) {
    return Vector(x * std::cos(angle) - y * std::sin(angle), y * std::cos(angle) + x * std::sin(angle));
}