#include "vector.hpp"
#include <cmath>
#include <cstdio>

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

double Vector2D::getMagnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const {
    return Vector2D(x + rhs.x, y + rhs.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

double Vector2D::dot(const Vector2D& rhs) const {
    return x * rhs.x + y * rhs.y;
}

void Vector2D::print() const {
    std::printf("<%.2f, %.2f>", x, y);
}

