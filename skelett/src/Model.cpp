#include "Model.hpp"

// Konstruktor
Model::Model()
{
    /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
    GLVector mRotation;
    GLVector mTranslation;
    GLVector mScale;
    GLMatrix mMatrix;
}

// Setter für das Material
void Model::setMaterial(Material material)
{
    mMaterial = Material();
    mMaterial.smooth = material.smooth;
    mMaterial.reflection = material.reflection;
    mMaterial.refraction = material.refraction;
    mMaterial.transparency = material.transparency;
    mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

/* Aufgabenblatt 2, Aufgabe 3: Implementieren Sie die vier Methoden für die Transformationen hier */
void Model::setRotation(GLVector rotation)
{
    //Winkel Phi für die jeweillige Rotation um eine Achse von Grad in Bogenmaß umrechnen
    mRotation(0) = rotation(0) * 2 * 3.1415 / 360;
    mRotation(1) = rotation(1) * 2 * 3.1415 / 360;
    mRotation(2) = rotation(2) * 2 * 3.1415 / 360;

    updateMatrix();
}

void Model::setScale(GLVector scale)
{
    for (int i = 0; i < 3; i++)
    {
        mScale(i) = scale(i);
    }
    updateMatrix();
}


void Model::setTranslation(GLVector translation)
{
    for (int i = 0; i < 3; i++)
    {
        mTranslation(i) = translation(i);
    }
    updateMatrix();
}

void Model::updateMatrix()
{
    //Rotations Matrizen
    GLMatrix rxMatrix;
    GLMatrix ryMatrix;
    GLMatrix rzMatrix;

    //Vector in Matrix zum berechnen
    GLMatrix sMatrix;
    for (int i = 0; i < 3; i++)
    {
        sMatrix.setValue(i, i, mScale(i));
    }
    //Vector in Matrix zum berechnen
    GLMatrix rMatrix;

    //Rotationsmatrix für Drehung um x-Achse
    rxMatrix.setValue(1, 1, cos(mRotation(0)));
    rxMatrix.setValue(1, 2, -sin(mRotation(0)));
    rxMatrix.setValue(2, 1, sin(mRotation(0)));
    rxMatrix.setValue(2, 2, cos(mRotation(0)));

    //Rotationsmatrix für Drehung um y-Achse
    ryMatrix.setValue(0, 0, cos(mRotation(1)));
    ryMatrix.setValue(0, 2, sin(mRotation(1)));
    ryMatrix.setValue(2, 0, -sin(mRotation(1)));
    ryMatrix.setValue(2, 2, cos(mRotation(1)));

    //Rotationsmatrix für Drehung um z-Achse
    rzMatrix.setValue(0, 0, cos(mRotation(2)));
    rzMatrix.setValue(0, 1, -sin(mRotation(2)));
    rzMatrix.setValue(1, 0, sin(mRotation(2)));
    rzMatrix.setValue(1, 1, cos(mRotation(2)));

    //Rotationsvector berechnen
    rMatrix = rxMatrix * ryMatrix * rzMatrix;

    //Vector in Matrix zum berechnen
    GLMatrix tMatrix;
    for (int i = 0; i < 3; i++)
    {
        tMatrix.setValue(i, 3, mTranslation(i));
    }

    //skalierte Matrix des Models
    mMatrix = tMatrix * rMatrix * sMatrix;
    //std::cout<< mMatrix << "\n" << mTranslation << "\n\n";
}

GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
