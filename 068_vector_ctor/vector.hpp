#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector2D {
private:
    double x;
    double y;

public:
    Vector2D();
    Vector2D(double x, double y);
    double getMagnitude() const;
    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D& operator+=(const Vector2D& rhs);
    double dot(const Vector2D& rhs) const;
    void print() const;
};

#endif




