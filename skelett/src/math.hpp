#pragma once

#include <array>
#include <vector>
#include <limits>
#include <memory>

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

/** Aufgabenblatt 2, Aufgabe 2
 *
 *GLPoint - Ortsvektor
 *GLVektor - Richtungsvektor
 * **/

/** Matrix und Vektor **/
inline GLVector operator*(const GLMatrix &m, const GLVector &rhs) {
  double x = m(0,0)*rhs(0) + m(0,1)*rhs(1) + m(0,2)*rhs(2);
  double y = m(1,0)*rhs(0) + m(1,1)*rhs(1) + m(1,2)*rhs(2);
  double z = m(2,0)*rhs(0) + m(2,1)*rhs(1) + m(2,2)*rhs(2);
  return GLVector(x, y, z);
}

/** Matrix und Point **/
inline GLPoint operator*(const GLMatrix &m, const GLPoint &p) {
  double x = m(0,0)*p(0) + m(0,1)*p(1) + m(0,2)*p(2) + m(0,3)*1;
  double y = m(1,0)*p(0) + m(1,1)*p(1) + m(1,2)*p(2) + m(1,3)*1;
  double z = m(2,0)*p(0) + m(2,1)*p(1) + m(2,2)*p(2) + m(2,3)*1;
  return GLPoint(x, y, z);
}

/** Matrix und Matrix **/
inline GLMatrix operator*(const GLMatrix &a, const GLMatrix &b) {
  GLMatrix produkt;
  for (int reihe = 0; reihe < 4; ++reihe) {
    for (int spalte = 0; spalte < 4; ++spalte) {
      double wert = 0;
      for (int k = 0; k < 4; ++k) {
        wert += a(reihe, k) * b(k, spalte);
      }
      produkt.setValue(reihe,spalte,wert);
    }
  }
  return produkt;
}



inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs)
{
  return GLVector(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator*(const GLPoint &lhs, double scale) {
  return GLPoint(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLPoint operator*(double scale, const GLPoint &rhs) {
  return GLPoint(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator+(const GLPoint &p1, const GLVector &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLPoint operator+(const GLPoint &p1, const GLPoint &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator+(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator-(const GLPoint &p1, const GLPoint &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector crossProduct(const GLVector &lhs, const GLVector &rhs) {
  return GLVector(lhs(1) * rhs(2) - lhs(2) * rhs(1),
                  lhs(2) * rhs(0) - lhs(0) * rhs(2),
                  lhs(0) * rhs(1) - lhs(1) * rhs(0));
}



inline double dotProduct(const GLVector &lhs, const GLVector &rhs) {
  return lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
}


inline int sgn(int x) { return (x > 0) ? 1 : (x < 0) ? -1 : 0; }

inline GLVector operator-(const GLVector &v1, const GLVector &v2) {
  return GLVector(v1(0) - v2(0), v1(1) - v2(1), v1(2) - v2(2));
}





