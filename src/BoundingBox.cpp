#include "BoundingBox.h"

BoundingBox::BoundingBox(Vector* p1, Vector* p2) {
    this->p1 = p1;
    this->p2 = p2;
}

void BoundingBox::expand(BoundingBox* other) {
    p1->x = min(p1->x, other->p1->x);
    p1->y = min(p1->y, other->p1->y);
    p1->z = min(p1->z, other->p1->z);

    p2->x = max(p2->x, other->p2->x);
    p2->y = max(p2->y, other->p2->y);
    p2->z = max(p2->z, other->p2->z);
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

Intersection* BoundingBox::intersect(Ray& r, Shape* s) {
    return intersect(r);
}

Intersection* BoundingBox::intersect(Ray& r) {
    double t1x = (p1->x - r.origin.x) / r.direction.x;
    double t2x = (p2->x - r.origin.x) / r.direction.x;

    double t1y = (p1->y - r.origin.y) / r.direction.y;
    double t2y = (p2->y - r.origin.y) / r.direction.y;
    
    double t1z = (p1->z - r.origin.z) / r.direction.z;
    double t2z = (p2->z - r.origin.z) / r.direction.z;
    
    if (t1x > t2x) swap(t1x, t2x);
    if (t1y > t2y) swap(t1y, t2y);
    if (t1z > t2z) swap(t1z, t2z);

    double t1 = max(t1x, max(t1y, t1z));
    double t2 = min(t2x, min(t2y, t2z));

    if (t2 < 0) return nullptr;
    if (t1 <= t2) {
        Intersection::DUMMY.ray_t = t1;
        return &Intersection::DUMMY;
    }
    return nullptr;
}

int BoundingBox::getLongestAxis() {
    double _max = max(p2->x - p1->x, max(p2->y - p1->y, p2->z - p1->z));
    if (_max == p2->x - p1->x) return xaxis;
    if (_max == p2->y - p1->y) return yaxis;
    return zaxis;
}

BoundingBox* BoundingBox::split(double value, bool isGreaterThan, int axis) {
    if (isGreaterThan) {
        switch(axis) {
        case xaxis: return new BoundingBox(new Vector(max(value, p1->x), p1->y, p1->z), p2);
        case yaxis: return new BoundingBox(new Vector(p1->x, max(value, p1->y), p1->z), p2);
        case zaxis: return new BoundingBox(new Vector(p1->x, p1->y, max(value, p1->z)), p2);
        }
    } else {
        switch(axis) {
        case xaxis: return new BoundingBox(p1, new Vector(min(value, p2->x), p2->y, p2->z));
        case yaxis: return new BoundingBox(p1, new Vector(p2->x, min(value, p2->y), p2->z));
        case zaxis: return new BoundingBox(p1, new Vector(p2->x, p2->y, min(value, p2->z)));
        }
    }

    return nullptr;
}
