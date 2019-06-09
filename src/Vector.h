#ifndef VECTOR_H_
#define VECTOR_H_

#include "math.h"
#include <iostream>

class Vector {
public:
    double x, y, z;
    Vector() : x(0.0), y(0.0), z(0.0) {}
    Vector(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector(Vector end, Vector begin);
    Vector subtract(const Vector& other) const;
    Vector add(const Vector& other) const;
    double dot(const Vector& other) const;
    Vector multiply(double k) const;
    Vector* multiplyInPlace(double k);
    Vector cross(const Vector& o) const;
    double norm() const;
    Vector normalize() const;

    static Vector zero;
};

std::ostream& operator<<(std::ostream &strm, const Vector &a);

#endif
