#include "Vector.h"

Vector::Vector(Vector end, Vector begin) {
    x = end.x - begin.x;
    y = end.y - begin.y;
    z = end.z - begin.z;
}
Vector Vector::subtract(const Vector& o) const{
    return Vector(x-o.x, y-o.y, z-o.z);
}
Vector Vector::add(const Vector& o) const {
    return Vector(x+o.x, y+o.y, z+o.z);
}

double Vector::dot(const Vector& o) const {
    return x*o.x + y*o.y + z*o.z;
}

Vector Vector::multiply(double k) const {
    return Vector(x*k, y*k, z*k);
}

Vector* Vector::multiplyInPlace(double k) {
    x = x*k;
    y = y*k;
    z = z*k;
    return this;
}

Vector Vector::cross(const Vector& o) const {
    return Vector(
        y*o.z - z*o.y,
        z*o.x - x*o.z,
        x*o.y - y*o.x
    );
}

double Vector::norm() const {
    return sqrt(x*x + y*y + z*z);
}

Vector Vector::normalize() const {
    double nrm = norm();
    if (nrm == 0) return multiply(1);
    return multiply(1.0 / nrm);
}

std::ostream& operator<<(std::ostream &strm, const Vector &a) {
    return strm << "(" << a.x << " " << a.y << " " << a.z << ")";
}

Vector Vector::zero(0, 0, 0);
