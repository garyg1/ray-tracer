#ifndef SHAPE_H_
#define SHAPE_H_

#include "Color.h"
#include "Intersection.h"
#include "BoundingBox.h"
#include "Vector.h"
#include "Intersectable.h"

#include <iostream>

class BoundingBox;

/* interface */
struct Shape : public Intersectable {
    Color color;
    double ka, kl, ks, kg;
    int phongExp;
    bool isGlazed = false;

    Shape() {
        Color c(50, 50, 50);
        color = c;
        ka = 0.5;
        kl = 0.5;
        ks = 0.3;
        kg = 0.0;
        phongExp = 20;
    }
    
    virtual std::string toString()=0;
    Intersection* intersect(Ray&) { return nullptr; }
    inline Intersection* intersect(Ray& r, Shape* s) {
        return (s == this)? nullptr : intersect(r);
    }
    virtual BoundingBox* getBoundingBox() const=0;
    virtual Vector getMidpoint()=0;
    virtual Vector* getMidpointPtr()=0;
};
#endif
