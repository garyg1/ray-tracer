#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "Intersectable.h"
#include "Vector.h"
#include "Ray.h"
#include "Settings.h"

class BoundingBox: public Intersectable {
public:
    Vector* p1;
    Vector* p2;

    BoundingBox(Vector* p1, Vector* p2);
    ~BoundingBox() {
        delete p1;
        delete p2;
    }
    void expand(BoundingBox*);
    Intersection* intersect(Ray&);
    Intersection* intersect(Ray&, Shape*);
    BoundingBox* split(double value, bool isGreaterThan, int axis);
    int getLongestAxis();
};


#endif
