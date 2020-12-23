#include "ellipse.h"
#include <cmath>

Ellipse::Ellipse(const Point& f1, const Point& f2, double d) : F1(f1), F2(f2) {
    F1 = (f1 < f2) ? f1 : f2;
    F2 = (f1 < f2) ? f2 : f1;
    dist = d;
}

std::pair <Line, Line> Ellipse::directrices() const {
    double k = eccentricity() / dist * 2;
    Vector F1F2 = Vector(F1, F2);
    F1F2.normalize();
    Point directrice1 = center() + F1F2 * k;
    Point directrice2 = center() - F1F2 * k;
    return std::make_pair(Line(directrice1, directrice1 + F1F2.getNormal()), Line(directrice2, directrice2 + F1F2.getNormal()));
}

std::pair<Point, Point> Ellipse::focuses() const {
    return std::make_pair(F1, F2);
}

double Ellipse::eccentricity() const {
    return Point::dist(F1, F2) / dist;
}

Point Ellipse::center() const {
    return Shape::centerOfSegment(F1, F2);
}

double Ellipse::perimeter() const {
    double majorAxis = dist / 2;
    double minAxis = getMinorAxis();

    return 4 * (PI * minAxis * majorAxis + (majorAxis - minAxis))
           / (majorAxis + minAxis);
}

double Ellipse::area() const {
    return PI * getMinorAxis() * dist / 2;
}

bool Ellipse::operator==(const Shape& another) const {
    const auto* anotherE = dynamic_cast<const Ellipse*>(&another);
    if (anotherE == nullptr) {
        return false;
    }
    return F1 == anotherE->F1 &&
           F2 == anotherE->F2 &&
           dist == anotherE->dist;
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    const auto* anotherE = dynamic_cast<const Ellipse*>(&another);
    if (anotherE == nullptr) {
        return false;
    }
    return Point::dist(F1, F2) == Point::dist(anotherE->F1, anotherE->F2) &&
           dist == anotherE->dist;
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    const auto* anotherE = dynamic_cast<const Ellipse*>(&another);
    if (anotherE == nullptr) {
        return false;
    }
    return dist / anotherE->dist == getMinorAxis() / anotherE->dist * 2;
}

bool Ellipse::containsPoint(const Point& point) const {
    Point center = this->center();
    Point shift = point - Vector(center);
    return Point::dist(shift, F1) + Point::dist(shift, F2) <= dist;
}

void Ellipse::rotate(const Point& center, double angle) {
    F1 = rotatePoint(F1, center, angle);
    F2 = rotatePoint(F2, center, angle);
}

void Ellipse::reflex(const Point& center) {
    F1 = reflectPoint(F1, center);
    F2 = reflectPoint(F2, center);
}

void Ellipse::reflex(const Line& axis) {
    F1 = reflectPoint(F1, axis);
    F2 = reflectPoint(F2, axis);
}

void Ellipse::scale(const Point& center, double scale) {
    Point point_on_ellipse = this->center() + Vector(0, dist / 2);
    Point scaledPoint = scalePoint(point_on_ellipse, center, scale);
    F1 = scalePoint(F1, center, scale);
    F2 = scalePoint(F2, center, scale);
    dist = Point::dist(F1, scaledPoint) + Point::dist(F2, scaledPoint);
}

double Ellipse::getMinorAxis() const {
    return std::sqrt(std::pow(dist / 2, 2) - std::pow(Point::dist(F1, F2) / 2, 2));
}