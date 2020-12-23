#pragma once
#include "rectangle.h"
#include "circle.h"

class Square : public Rectangle {
public:
    Square(Point p1, Point p2) : Rectangle(p1, p2, 1) {}
    Circle circumscribedCircle();
    Circle inscribedCircle();
};