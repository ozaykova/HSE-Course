#include <utility>
#include <cmath>
#include "polygon.h"
#include "ellipse.h"

Polygon::Polygon(std::vector<Point> p) : points(std::move(p)) {}

int Polygon::verticesCount() const {
    return points.size();
}

std::vector<Point> Polygon::getVertices() const {
    return points;
}

double Polygon::perimeter() const {
    double res = 0;
    for (size_t i = 0; i < points.size(); i++) {
        res += Point::dist(points[i], points[(i + 1) % points.size()]);
    }
    return res;
}

double Polygon::area() const {
    double area = 0;
    for (size_t i = 0; i < points.size() - 1; i++) {
        area += points[i].x * points[i + 1].y;
    }
    for (size_t i = 0; i < points.size() - 1; i++) {
        area -= points[i + 1].x * points[i].y;
    }
    area -= points[0].x * points[points.size() - 1].y - points[points.size() - 1].x * points[0].y;
    return std::abs(area * 0.5);
}

bool Polygon::operator==(const Shape& another) const {
    const auto* anotherP = dynamic_cast<const Polygon*>(&another);
    if (anotherP == nullptr) {
        return false;
    }
    if (anotherP->verticesCount() != verticesCount()) {
        return false;
    }

    for (size_t i = 0; i < points.size(); i++) {
        if (anotherP->getVertices()[i] == points[(i + 1) % points.size()]) {
            return false;
        }
    }
    return true;
}

bool Polygon::isCongruentTo(const Shape& another) const {
    const auto* anotherP = dynamic_cast<const Polygon*>(&another);
    if (anotherP == nullptr) {
        return false;
    }
    
    if (anotherP->verticesCount() != verticesCount()) {
        return false;
    }

   for (size_t i = 0; i < points.size() - 1; i++) {
        Vector a(points[i], points[i + 1]);
        Vector b(anotherP->getVertices()[i], anotherP->getVertices()[i + 1]);
        if (Vector::vectorComposition(a, b) == 0 && (a * b) == 0) {
            return false;
        }
    }
    return true;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    const auto* anotherP = dynamic_cast<const Polygon*>(&another);
    if (anotherP == nullptr || (verticesCount() != anotherP->verticesCount())) {
        return false;
    }
    if (verticesCount() == 1) {
        return true;
    }

    for (size_t i = 0; i < points.size() - 1; ++i) {
        if (Point::dist(anotherP->getVertices()[i], anotherP->getVertices()[i]) !=
            Point::dist(getVertices()[i], getVertices()[i])) {
            return false;
        }
    }
    return true;
}

void Polygon::reflex(const Line& axis) {
    std::vector<Point> tmp;
    for (auto& point : points) {
        tmp.push_back(reflectPoint(point, axis));
    }
    points = tmp;
}

void Polygon::reflex(const Point& center) {
    std::vector<Point> tmp;
    for (auto& point : points) {
        tmp.push_back(reflectPoint(point, center));
    }
    points = tmp;
}

void Polygon::scale(const Point& center, double scale) {
    std::vector<Point> tmp;
    for (auto& point : points) {
        tmp.push_back(scalePoint(point, center, scale));
    }
    points = tmp;
}

bool Polygon::containsPoint(const Point& point) const {
    for (size_t i = 0; i < points.size() - 1; i++) {
        Vector a = Vector(points[i], point);
        Vector b = Vector(points[(i + 1) % points.size()], point);
        if (Vector::vectorComposition(a, b) == 0 || a * b <= 0) {
            return true;
        }
    }
    return false;
}

void Polygon::rotate(const Point& center, double angle) {
    std::vector<Point> tmp;
    for (auto point : points) {
        tmp.push_back(rotatePoint(point, center, angle * PI / 180));
    }
    points = tmp;
}

bool Polygon::isConvex() const {
    if (points.size() < 3) {
        return false;
    }
    std::vector<Point> tmp = points;
    Point a(tmp[0]), b(tmp[1]), c(tmp[2]);
    tmp.push_back(tmp[0]);
    bool sign = Vector::vectorComposition(Vector(a, b), Vector(a, c)) > 0;
    for (size_t i = 3; i < tmp.size(); i++) {
        a = b;
        b = c;
        c = tmp[i];
        if ((Vector::vectorComposition(Vector(a, b), Vector(a, c)) > 0) != sign) {
            return false;
        }
    }
    return true;
}