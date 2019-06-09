#ifndef COLOR_H_
#define COLOR_H_

#include "Settings.h"

struct Color {
    static Color WHITE, RED, GREEN, BLUE, BLACK, LIGHTBLUE;
    int r, g, b;
    Color() {r = 0; g = 0; b = 0;}
    Color(int r, int g, int b) : r(r), g(g), b(b) {}
    Color(Color* c) : r(c->r), g(c->g), b(c->b) {}
    bool operator==(const Color&);

    static Color* ScaleColor(Color*, double);
    static Color* AddColors(Color*, Color*, double);
};

#endif
