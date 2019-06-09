#include "Intersection.h"

Intersection::~Intersection() {
}

Intersection Intersection::DUMMY(nullptr, Vector::zero, Vector::zero, nullptr, __DBL_MIN__);


std::ostream& operator<<(std::ostream &strm, const Intersection &a) {
    return strm << "Intersection(" << a.shape->toString() << ", " << a.point << ", " << a.normal << ", " << a.ray_t << ")";
}
