// Model.hpp Ergänzung
#pragma once

#include <vector> // Sicherstellen, dass vector enthalten ist
#include "Material.hpp"
#include "math.hpp"
#include "structs.hpp"

class Model {
public:
    std::vector<Triangle> mTriangles;
    GLMatrix mTransform; // Transformationsmatrix
    GLVector mRotation;
    GLVector mTranslation;
    GLVector mScale;
    Material mMaterial; // Hinzugefügte Material-Membervariable

    Model() : mRotation(0.0f, 0.0f, 0.0f), mTranslation(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
        updateMatrix();
    }

    void setRotation(const GLVector& rotation);
    void setTranslation(const GLVector& translation);
    void setScale(const GLVector& scale);
    void updateMatrix();

    // Hinzugefügte Setter/Getter für Material
    void setMaterial(Material material);
    Material getMaterial() const;
};