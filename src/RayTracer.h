#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "Vector.h"
#include "Shape.h"
#include "Color.h"
#include "Intersection.h"
#include "Intersectable.h"
#include "Ray.h"
#include "Light.h"
#include "View.h"
#include "Color.h"
#include "RayTracerSettings.h"

#include "Settings.h"

#include <vector>
#include <iostream>

using namespace std;

struct RayTracerSettings;
struct View;

class RayTracer {
    Intersectable* objects;
    vector<Light*> lights;
    View* view;
    RayTracerSettings* rts;

public:
    RayTracer(Intersectable* shapes, vector<Light*> lights, View* view, RayTracerSettings* rts) 
        : objects(shapes), lights(lights), view(view), rts(rts) {}

    ~RayTracer() {
        delete view;
        delete rts;
    }

    void solve(unsigned int img[H][W][3]);

private: 
    Color* getColor(Intersection*);
    Color* getColorRecursive(Intersection*, int);
    Vector* getXYZ(int, int);
    Vector* getXYZParallel(int, int);
    Color* getBG() { return new Color(rts->BG); }
};

#endif
