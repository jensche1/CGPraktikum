#pragma once

#include "Color.hpp"

struct Material {
 public:
  Material()
      : smooth(false),
        reflection(0.0),
        refraction(0.0),
        transparency(0.0),
        color(Color(0, 0, 0)),
   //  4.2
        ambient(0.1, 0.1, 0.1),     // Umgebungslicht-Anteil
        diffuse(0.9, 0.9, 0.9),     // Diffuser Anteil
        specular(0.3, 0.3, 0.3),        // Spiegelnder Anteil
        shininess(20.0f)            // Glanz-Exponent (alpha)
    //  4.2
{}

  bool smooth;
  float reflection;
  float refraction;
  float transparency;
  Color color;

    //  4.2
    Color ambient   = Color(0.1, 0.1, 0.1);
    Color diffuse   = Color(0.9, 0.9, 0.9);
    Color specular  = Color(0.3, 0.3, 0.3);
    float shininess = 20.0f;
    //  4.2
};
