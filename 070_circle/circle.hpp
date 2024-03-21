#ifndef CIRCLE_HPP
#define CIRCLE_HPP

class Point {
private:
    double x;
    double y;

public:
    Point();
    Point(double x, double y);
    void move(double dx, double dy);
    double getX() const;
    double getY() const;
};

class Circle {
private:
    Point center;
    double radius;

public:
    Circle(Point center, double radius);
    void move(double dx, double dy);
    double intersectionArea(const Circle& other) const;
};

#endif

