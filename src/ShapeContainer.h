#ifndef SHAPE_CONTAINER_H_
#define SHAPE_CONTAINER_H_

#include "Intersectable.h"
#include "Shape.h"
#include "Ray.h"

class ShapeContainer: public Intersectable {

public:
    std::vector<Shape*> shapes;
    ShapeContainer() {}
    ShapeContainer(const std::vector<Shape*>& shapes) {
        for (int i = 0; i < shapes.size(); i++) {
            this->shapes.push_back(shapes[i]);
        }
    }
    inline Intersection* intersect(Ray& r) { return r.intersect(shapes); }
    inline Intersection* intersect(Ray& r, Shape* ignore) { return r.intersect(shapes, ignore); }
};

#endif
