#ifndef _INTERSECTABLE_H
#define _INTERSECTABLE_H

class Intersection;
class Ray;
class Shape;
#include <iostream>
class Intersectable {
public:
    virtual Intersection* intersect(Ray&)=0;
    virtual Intersection* intersect(Ray& r, Shape* ignore)=0;
};



#endif
