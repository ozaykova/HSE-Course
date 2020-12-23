#pragma once
#include <utility>

#include "shape.h"

class Ellipse : public Shape {
public:
    Ellipse(const Point& f1, const Point& f2, double d);
    std::pair<Point,Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    double eccentricity() const;
    Point center() const;

    double perimeter() const override;
    double area() const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    bool operator==(const Shape& another) const override;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(const Point& point) const override;
    void scale(const Point& center, double scale) override;

protected:
    double dist;

private:
    Point F1;
    Point F2;

    double getMinorAxis() const;
};