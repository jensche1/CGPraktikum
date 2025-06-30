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

// Cora 4.2

// Definiert, wie eine Farbe mit einer Zahl multipliziert wird (z.B. Color(1,0,0) * 0.5)
inline Color operator*(const Color& c, double s) {
    return Color(c.r * s, c.g * s, c.b * s);
}

// Stellt sicher, dass die Multiplikation auch andersherum funktioniert (z.B. 0.5 * Color(1,0,0))
inline Color operator*(double s, const Color& c) {
    return c * s;
}

// Cora 4.2
