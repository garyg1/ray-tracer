#include "Color.h"

/*
 * Implementation
 */
bool Color::operator==(const Color& other) {
    return (r == other.r) && (g == other.g) && (b == other.b);
}

Color* Color::ScaleColor(Color* c, double k) {
    if (c == nullptr) return nullptr;
    return new Color(
        min((int) (c->r * k), 255),
        min((int) (c->g * k), 255),
        min((int) (c->b * k), 255)
    );
}

Color* Color::AddColors(Color* c, Color* c2, double weight) {
    return new Color(
        min((int) (c->r + weight*c2->r), 255),
        min((int) (c->g + weight*c2->g), 255),
        min((int) (c->b + weight*c2->b), 255)
    );
}

Color Color::WHITE(255, 255, 255);
Color Color::RED(255, 80, 100);
Color Color::GREEN(80, 255, 120);
Color Color::BLUE(100, 100, 255);
Color Color::LIGHTBLUE(20, 143, 239);
Color Color::BLACK(0, 0, 0);
