#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Vector.h"
#include "Shape.h"
#include "Color.h"
#include "Intersection.h"
#include "Ray.h"
#include "Settings.h"

#include <math.h>
#include <iostream>

class Triangle : public Shape {
public:
    // vertices
    Vector a;
    Vector b;
    Vector c;

    Triangle(Vector a, Vector b, Vector c, Color col)
        : a(a), b(b), c(c) {this->color = col;}
    Triangle(Vector a, Vector b, Vector c, Color col, double ka, double kl, double ks, double kg = 0, bool isGlazed=false, int phongExp = 20) 
        : a(a), b(b), c(c) {this->color = col; this->ka = ka; this->kl = kl; this->ks = ks; this->kg = kg; this->isGlazed=isGlazed; this->phongExp=phongExp;}
    Intersection* intersect(Ray& r);
    std::string toString();
    BoundingBox* getBoundingBox() const;
    Vector getMidpoint();
    Vector* getMidpointPtr();
};

#endif
