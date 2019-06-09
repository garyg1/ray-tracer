#ifndef VIEW_H_
#define VIEW_H_

#include "Vector.h"

struct View {
    // origin
    Vector e;

    // camera basis {u, v, w}
    Vector u;
    Vector v;
    Vector w;

    View(Vector e, Vector u, Vector v, Vector w) 
        : e(e), u(u), v(v), w(w) { }
    
    Vector* getXYZ(double a, double b, double c) {
        return new Vector(
            e.x + a*u.x + b*v.x + c*w.x,
            e.y + a*u.y + b*v.y + c*w.y,
            e.z + a*u.z + b*v.z + c*w.z
        );
    }
};

#endif
