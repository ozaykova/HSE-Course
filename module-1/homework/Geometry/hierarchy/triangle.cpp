#include "triangle.h"
#include <cmath>

Circle Triangle::circumscribedCircle()  {
    Point a = points[0];
    Point b = points[1];
    Point c = points[2];
    double dest = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

    double x = (a.x * a.x + a.y * a.y * (b.y - c.y) +
                b.x * b.x + b.y * b.y * (c.y - a.y) +
                c.x * c.x + c.y * c.y * (a.y - b.y)) / dest;

    double y = (a.x * a.x + a.y * a.y * (c.x - b.x) +
                b.x * b.x + b.y * b.y * (a.x - c.x) +
                c.x * c.x + c.y * c.y * (b.x - a.x)) / dest;
    return Circle(Point(x, y), Point::dist(a, Point(x, y)));
}

Circle Triangle::inscribedCircle() {
    Point a = points[0];
    Point b = points[1];
    Point c = points[2];
    double x = (Point::dist(b, c) * a.x + Point::dist(c, a) * b.x + Point::dist(a, b) * c.x)
            / (Point::dist(b, c) + Point::dist(c, a) + Point::dist(a, b));

    double y = (Point::dist(b, c) * a.y + Point::dist(c, a) * b.y + Point::dist(a, b) * c.y)
            / (Point::dist(b, c) + Point::dist(c, a) + Point::dist(a, b));
    double radius = area() / (perimeter() / 2);
    return Circle(Point(x, y), radius);
}