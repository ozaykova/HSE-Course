#pragma once
const static double PI = 3.1415926;
const static double EPS = 1e-6;

struct Point {
    Point(double a, double b);
    double x;
    double y;

    friend bool operator==(const Point &a, const Point &b);
    friend bool operator!=(const Point &a, const Point &b);
    friend bool operator<(const Point &a, const Point &b);

    static double dist(const Point& a, const Point& b);
};