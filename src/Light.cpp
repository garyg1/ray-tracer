#include "Light.h"

Vector Light::getDirection(Vector start) {
    switch(type) {
    case Light::PARALLEL:
        return direction;
    case Light::POINT:
        return Vector(start, point);
    default:
        throw std::runtime_error("Invalid state: Invalid Light::type");
    }
}

double Light::getIntensity(Intersection* i) {
    if (i == nullptr) return 0.0;
    switch(type) {
    case Light::PARALLEL:
        return intensity;
    case Light::POINT:
        Vector direction(i->point, point);
        double dist = direction.norm();
        return intensity / (dist*dist);
    }
}
