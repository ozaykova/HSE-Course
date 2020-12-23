#pragma once
#include "point.h"

struct Vector {
    Vector(double a, double b);
    Vector(const Point& point);
    Vector(const Point& A, const Point& B);
    double x;
    double y;

    Vector getNormal() const;
    void normalize();

    friend Vector operator*(const Vector& v, double s);
    friend Point operator+(const Point& p, const Vector& v);
    friend double operator*(const Vector& v1, const Vector& v2);
    friend Point operator-(const Point& p, const Vector& v);

    Vector getRotatedVector(double angle);
    static double vectorComposition(const Vector& v1, const Vector& v2);
    double getLength() const;
};