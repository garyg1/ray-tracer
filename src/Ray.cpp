#include "Ray.h"
#include <iostream>
std::ostream& operator<<(std::ostream &strm, const Ray &a) {
    return strm << "Ray(" << a.origin << " " << a.direction << ")";
}

Intersection* Ray::intersect(std::vector<Shape*>& objects) {
    return intersect(objects, nullptr);
}

Intersection* Ray::intersect(std::vector<Shape*>& objects, Shape* ignore) {
    t = __DBL_MAX__;
    Intersection* closest = nullptr;

    for (Shape* s : objects) {
        if (s == ignore) continue;
        
        Intersection* i = s->intersect(*this);
        if (i != nullptr && i->ray_t < t) {
            t = i->ray_t;
            delete closest;
            closest = i;
        } else {
            delete i;
        }
    }

    return closest;
}

Vector Ray::eval(double t) {
    Vector v = origin.add(direction.multiply(t));
    return v;

}
