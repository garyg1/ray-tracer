#ifndef RAY_H_
#define RAY_H_

#include "Shape.h"
#include "Intersectable.h"
#include "Intersection.h"
#include "Vector.h"

#include <vector>
#include <iostream>

using namespace std;

class Ray {
public:
    Vector origin;
    Vector direction;
    double t;
    
    Ray(Vector origin, Vector direction) : origin(origin), direction(direction) {
        t = __DBL_MAX__;
    }
    Ray(const Ray& r) {
        t = __DBL_MAX__;
        origin = r.origin;
        direction = r.direction;
    }

    Intersection* intersect(vector<Shape*>& objects);
    Intersection* intersect(vector<Shape*>& objects, Shape* ignore);
    Vector eval(double t);
};

#endif
