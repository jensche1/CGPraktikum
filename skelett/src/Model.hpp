#pragma once

#include "Material.hpp"
#include "math.hpp"
#include "structs.hpp"

class Model {
 public:
public:
    std::vector<Triangle> mTriangles;
    GLMatrix mTransform; // Transformationsmatrix
    GLVector mRotation;
    GLVector mTranslation;
    GLVector mScale;

    Model() : mRotation(0.0f, 0.0f, 0.0f), mTranslation(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
        updateMatrix();
    }

    void setRotation(const GLVector& rotation);
    void setTranslation(const GLVector& translation);
    void setScale(const GLVector& scale);
    void updateMatrix();

};
