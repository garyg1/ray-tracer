#ifndef RAY_TRACER_SETTINGS_H_
#define RAY_TRACER_SETTINGS_H_

#include "Color.h"

struct RayTracerSettings {
    Color* BG;
    double AMBIENT;
    bool doPerspective;
    int width = 4;
    int height = 3;

    RayTracerSettings(Color* bg, double amb, bool doP)
        : BG(bg), AMBIENT(amb), doPerspective(doP) {}
};

#endif
