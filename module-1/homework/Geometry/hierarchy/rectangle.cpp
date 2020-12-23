#include "rectangle.h"
#include "ellipse.h"
#include <cmath>

Point Rectangle::center() {
    return Shape::centerOfSegment(points[0], points[2]);
}

Rectangle::Rectangle(const Point& a, const Point& b, double ratio) : Polygon() {
    Point center = centerOfSegment(a, b);
    double diag = Point::dist(a, b);
    double alpha = std::acos(1 - (std::pow(sqrt(diag * diag / (ratio * ratio + 1)) * ratio, 2) / (2 * std::pow(Point::dist(a, center), 2))));
    points = std::vector<Point>{rotatePoint(a, center, alpha), a, rotatePoint(a, center, -(PI - alpha)), b};
}
