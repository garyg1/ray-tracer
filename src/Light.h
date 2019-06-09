#ifndef LIGHT_H_
#define LIGHT_H_

#include "Vector.h"
#include "Intersection.h"
#include <stdexcept>

class Light {
public:
    const static int PARALLEL = 0;
    const static int POINT = 1;

private:
    Vector point;
    Vector direction;
    double intensity;
    const int type;

public:
    Light(Vector point, Vector direction, const int type, double intensity) 
        : point(point), direction(direction.normalize()), type(type), intensity(intensity) {}
    Vector getDirection(Vector);
    double getIntensity(Intersection*);
};

#endif
