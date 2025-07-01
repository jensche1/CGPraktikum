#pragma once

#include <array>
#include <vector>
#include <limits>
#include <memory>

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs) {
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

//  4.2
inline GLVector operator-(const GLVector &v1, const GLVector &v2) {
  return GLVector(v1(0) - v2(0), v1(1) - v2(1), v1(2) - v2(2));
}
//  4.2
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

/** Aufgabenblatt 2, Aufgabe 2 **/

inline GLVector operator*(const GLMatrix &lhs, const GLVector &rhs) { // Transformation Richtungsvektor
  // Richtungsvektor: w = 0; keine Translation
  return GLVector(
      lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2),
      lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2),
      lhs(2, 0) * rhs(0) + lhs(2, 1) * rhs(1) + lhs(2, 2) * rhs(2)
  );
}

inline GLPoint operator*(const GLMatrix &lhs, const GLPoint &rhs) {  // Transformation Ortsvektor
  // Ortsvektor: w = 1; Translation berücktsichtigt
  double x = lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2) + lhs(0, 3);
  double y = lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2) + lhs(1, 3);
  double z = lhs(2, 0) * rhs(0) + lhs(2, 1) * rhs(1) + lhs(2, 2) * rhs(2) + lhs(2, 3);

  return GLPoint(x, y, z);
}

inline GLMatrix operator*(const GLMatrix &lhs, const GLMatrix &rhs) { // 4x4 Matrixmultiplikation
  GLMatrix result;
  for (int i = 0; i < 4; ++i) { // iterieren über i in lhs 
    for (int j = 0; j < 4; ++j) { // iterieren über j in rhs 
      double sum = 0;
      for (int k = 0; k < 4; ++k) { // iterieren über k für Multiplikation
        sum += lhs(i, k) * rhs(k, j); // Zeile i aus lhs mit Spalte j aus rhs und summieren 
      }
      result.setValue(i, j, sum);
    }
  }
  return result;
}