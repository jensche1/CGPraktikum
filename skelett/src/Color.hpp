#pragma once

/**
 ** Klasse für Farbdefinitionen
 ** Der Wertebereich je Farbkanal liegt zwischen 0.0 und 1.0
 **/
class Color {
public:
    Color();

    Color(float r, float g, float b);

    Color &operator*=(double i);

    Color &operator/=(double i);

    Color &operator+=(const Color &rhs);

    bool operator!=(const Color &rhs);

    float r, g, b;
};

Color operator*(const Color &lhs, double rhs);
Color operator*(double lhs, const Color &rhs);
Color operator*(const Color &lhs, const Color &rhs); // NEW: For Color * Color multiplication
Color operator+(const Color &lhs, const Color &rhs);