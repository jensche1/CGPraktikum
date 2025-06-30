#include "Color.hpp"

Color::Color() {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color &Color::operator*=(double i) {
    r *= i;
    g *= i;
    b *= i;
    return *this;
}

Color &Color::operator/=(double i) {
    double frac = 1.0 / i;
    r *= frac;
    g *= frac;
    b *= frac;
    return *this;
}

Color &Color::operator+=(const Color &rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;

    return *this;
}

bool Color::operator!=(const Color &rhs) {
    if( r != rhs.r or g != rhs.g or b != rhs.b )
        return true;
    else
        return false;
}

Color operator*(const Color &lhs, double rhs) {
    return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

Color operator*(double lhs, const Color &rhs) {
    return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b);
}

// NEW: Implementation for Color * Color multiplication
Color operator*(const Color &lhs, const Color &rhs) {
    return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

Color operator+(const Color &lhs, const Color &rhs) {
    return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}