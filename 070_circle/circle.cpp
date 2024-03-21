#include "circle.hpp"
#include <cmath>

Point::Point() : x(0), y(0) {}

Point::Point(double x, double y) : x(x), y(y) {}

void Point::move(double dx, double dy) {
    x += dx;
    y += dy;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

Circle::Circle(Point center, double radius) : center(center), radius(radius) {}

void Circle::move(double dx, double dy) {
    center.move(dx, dy);
}

double Circle::intersectionArea(const Circle& other) const {
    double dx = center.getX() - other.center.getX();
    double dy = center.getY() - other.center.getY();
    double d = std::sqrt(dx * dx + dy * dy);

    if (d >= radius + other.radius) {
        return 0.0;
    }

    if (d <= std::abs(radius - other.radius)) {
        double smallerRadius = (radius < other.radius) ? radius : other.radius;
        return M_PI * std::pow(smallerRadius, 2);
    }

    double alpha = 2 * std::acos((std::pow(radius, 2) + std::pow(d, 2) - std::pow(other.radius, 2)) / (2 * radius * d));
    double beta = 2 * std::acos((std::pow(other.radius, 2) + std::pow(d, 2) - std::pow(radius, 2)) / (2 * other.radius * d));

    double area1 = 0.5 * std::pow(radius, 2) * (alpha - std::sin(alpha));
    double area2 = 0.5 * std::pow(other.radius, 2) * (beta - std::sin(beta));

    return area1 + area2;
}


