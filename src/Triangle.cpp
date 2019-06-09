#include "Triangle.h"

Intersection* Triangle::intersect(Ray& r) {
        Vector v1(a, b);
        Vector v2(a, c);
        Vector n = v1.cross(v2);

        double denom = n.dot(r.direction);
        const double zero = 0.0000001;
        if (denom < zero && denom > -zero) {
            return nullptr;
        }

        Vector toTri(a, r.origin);
        double t = n.dot(toTri) / n.dot(r.direction);

        if (t < 0) { 
            return nullptr;
        }


        // ensure normal is in correct direction
        if (n.dot(r.direction) > 0) {
            n = n.multiply(-1);
        }
        
        Vector p = r.eval(t);
        
        Vector ap(p, a);
        Vector bp(p, b);
        Vector cp(p, c);
        
        Vector ab(b, a);
        Vector bc(c, b);
        Vector ca(a, c);

        double sign1 = ab.cross(ap).dot(n) > 0.0;
        double sign2 = bc.cross(bp).dot(n) > 0.0;
        double sign3 = ca.cross(cp).dot(n) > 0.0;

        if ((sign1 != sign2) || (sign2 != sign3)) {
            return nullptr;
        }
        
        return new Intersection(new Ray(r), p, n, this, t);
}


std::string Triangle::toString() {
    char buf[200];
    sprintf(buf, "Triangle: [%f %f %f], [%f %f %f], [%f %f %f]", a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
    return buf;
}

BoundingBox* Triangle::getBoundingBox() const {

    return new BoundingBox(
        new Vector(
            min(a.x, min(b.x, c.x)),
            min(a.y, min(b.y, c.y)),
            min(a.z, min(b.z, c.z))
        ),
        new Vector(
            max(a.x, max(b.x, c.x)),
            max(a.y, max(b.y, c.y)),
            max(a.z, max(b.z, c.z))
        )
    );
}

Vector Triangle::getMidpoint() {
    return Vector(
        (a.x + b.x + c.x) / 3.0,
        (a.y + b.y + c.y) / 3.0,
        (a.z + b.z + c.z) / 3.0
    );
}

Vector* Triangle::getMidpointPtr() {
    return new Vector(
        (a.x + b.x + c.x) / 3.0,
        (a.y + b.y + c.y) / 3.0,
        (a.z + b.z + c.z) / 3.0
    );
}
