#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include <iostream>
struct Intersection;

class Ray;
struct Shape;

#include "Vector.h"
#include "Ray.h"

struct Intersection {
    const Ray* r;
    Shape* shape;
    Vector point;
    Vector normal;
    double ray_t;
    
    Intersection(const Ray* r, Vector p, Vector n, Shape* s, double t)
        : r(r), shape(s), point(p), normal(n.normalize()), ray_t(t) {}

    ~Intersection();

    static Intersection* Min(Intersection* a, Intersection* b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        return (a->ray_t > b->ray_t)? a : b;
    }

    static Intersection DUMMY;
};

std::ostream& operator<<(std::ostream &strm, const Intersection &a);


#endif
