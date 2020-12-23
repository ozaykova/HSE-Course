#include "shape.h"
#include <cmath>


Point Shape::centerOfSegment(const Point& x, const Point& y) {
    return Point((x.x + y.x) / 2, (x.y + y.y) / 2);
}

Point Shape::reflectPoint(const Point& point, const Line& axis) {
    Vector perpendicular = axis.getDirectionVector().getNormal();
    perpendicular.normalize();
    perpendicular = perpendicular * (std::abs(Vector::vectorComposition(Vector(axis.x, point), Vector(axis.x, axis.y)))
            / Vector(axis.x, axis.y).getLength());
    return point + perpendicular * 2;
}

Point Shape::reflectPoint(const Point& p, const Point& center) {
    return p + (Vector(p, center) * 2);
}

Point Shape::scalePoint(const Point& p, const Point& center, double scale) {
    return center + Vector(center, p) * scale;
}

Point Shape::rotatePoint(const Point& p, const Point& center, double angle) {
    return center + Vector(center, p).getRotatedVector(angle);
}