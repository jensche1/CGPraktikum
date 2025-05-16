#include "Model.hpp"

void Model::setRotation(const GLVector& rotation) {
    mRotation = rotation;
    updateMatrix();
}

void Model::setTranslation(const GLVector& translation) {
    mTranslation = translation;
    updateMatrix();
}

void Model::setScale(const GLVector& scale) {
    mScale = scale;
    updateMatrix();
}

void Model::updateMatrix() {
    // Erstelle die Transformationsmatrizen
    GLMatrix scaleMatrix;
    scaleMatrix.setValue(0, 0, mScale(0));
    scaleMatrix.setValue(1, 1, mScale(1));
    scaleMatrix.setValue(2, 2, mScale(2));
    scaleMatrix.setValue(3,3,1);

    GLMatrix rotationXMatrix;
    rotationXMatrix.setValue(0, 0, 1);
    rotationXMatrix.setValue(1, 1, cos(mRotation(0) * M_PI / 180.0));
    rotationXMatrix.setValue(1, 2, -sin(mRotation(0) * M_PI / 180.0));
    rotationXMatrix.setValue(2, 1, sin(mRotation(0) * M_PI / 180.0));
    rotationXMatrix.setValue(2, 2, cos(mRotation(0) * M_PI / 180.0));
    rotationXMatrix.setValue(3,3,1);

    GLMatrix rotationYMatrix;
    rotationYMatrix.setValue(0, 0, cos(mRotation(1) * M_PI / 180.0));
    rotationYMatrix.setValue(0, 2, sin(mRotation(1) * M_PI / 180.0));
    rotationYMatrix.setValue(1, 1, 1);
    rotationYMatrix.setValue(2, 0, -sin(mRotation(1) * M_PI / 180.0));
    rotationYMatrix.setValue(2, 2, cos(mRotation(1) * M_PI / 180.0));
    rotationYMatrix.setValue(3,3,1);

    GLMatrix rotationZMatrix;
    rotationZMatrix.setValue(0, 0, cos(mRotation(2) * M_PI / 180.0));
    rotationZMatrix.setValue(0, 1, -sin(mRotation(2) * M_PI / 180.0));
    rotationZMatrix.setValue(1, 0, sin(mRotation(2) * M_PI / 180.0));
    rotationZMatrix.setValue(1, 1, cos(mRotation(2) * M_PI / 180.0));
    rotationZMatrix.setValue(2, 2, 1);
    rotationZMatrix.setValue(3,3,1);

    GLMatrix translationMatrix;
    translationMatrix.setValue(0, 0, 1);
    translationMatrix.setValue(0, 3, mTranslation(0));
    translationMatrix.setValue(1, 1, 1);
    translationMatrix.setValue(1, 3, mTranslation(1));
    translationMatrix.setValue(2, 2, 1);
    translationMatrix.setValue(2, 3, mTranslation(2));
    translationMatrix.setValue(3,3,1);


    // Kombiniere die Matrizen (Scale -> Rotate -> Translate)
    mTransform = translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;
}
