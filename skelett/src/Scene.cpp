// Scene.cpp Implementierung

#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include <limits> // Für std::numeric_limits

Scene::Scene() {}

/**
 * Lädt 3D-Modelle aus den angegebenen Dateien und fügt sie der Szene hinzu.
 * (Aufgabenblatt 1, Aufgabe 1)
 */
void Scene::load(const std::vector<std::string> &pFiles) {
  std::cout << "Laden der PLY Dateien gestartet." << std::endl;

  Assimp::Importer importer;
  for (const std::string &path : pFiles) {
    const aiScene *aiscene =
        importer.ReadFile(path, aiProcess_JoinIdenticalVertices |
                                    aiProcess_Triangulate |
                                    aiProcess_CalcTangentSpace);

    if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !aiscene->mRootNode) {
      std::cerr << "Assimp-Fehler beim Laden von " << path << ": "
                << importer.GetErrorString() << std::endl;
      continue;
    }

    Model model; // Erstelle ein neues Model-Objekt für jedes geladene Modell

    for (size_t i = 0; i < aiscene->mNumMeshes; ++i) {
      aiMesh *mesh = aiscene->mMeshes[i];

      // Kopiere die Vertices
      std::vector<GLPoint> vertices;
      for (size_t j = 0; j < mesh->mNumVertices; ++j) {
        vertices.emplace_back(mesh->mVertices[j].x, mesh->mVertices[j].y,
                              mesh->mVertices[j].z);
      }

      // Kopiere die Faces (Dreiecke)
      for (size_t j = 0; j < mesh->mNumFaces; ++j) {
        aiFace &face = mesh->mFaces[j];
        assert(face.mNumIndices == 3); // Stellen Sie sicher, dass es ein Dreieck ist

        Triangle tri;
          tri.vertex[0] = GLPoint(vertices[face.mIndices[0]](0), vertices[face.mIndices[0]](1), vertices[face.mIndices[0]](2));
          tri.vertex[1] = GLPoint(vertices[face.mIndices[1]](0), vertices[face.mIndices[1]](1), vertices[face.mIndices[1]](2));
          tri.vertex[2] = GLPoint(vertices[face.mIndices[2]](0), vertices[face.mIndices[2]](1), vertices[face.mIndices[2]](2));

        GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                       tri.vertex[2] - tri.vertex[0]);
        normal.normalize();
        tri.normal = normal;
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
        model.mTriangles.push_back(tri);
      }
    }
    // Jedes Model zum Vector der Models der Scene hinzufügen
    mModels.push_back(model);
  }

  std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 * (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    hitRecord.parameter = std::numeric_limits<double>::max(); // Initialisiere mit maximalem Wert
    bool hit = false;

    // Iteriere über alle Modelle (Dreiecke)
    for (size_t modelIdx = 0; modelIdx < mModels.size(); ++modelIdx) {
        Model& currentModel = mModels[modelIdx];
        // Transformation des Strahls in den lokalen Koordinatenraum des Modells
        // Ray_local.origin = M_inv * Ray_world.origin
        // Ray_local.direction = M_inv * Ray_world.direction (w=0 für Vektor)
        GLMatrix invTransform = currentModel.mTransform;
        invTransform.inverse(); // Berechne die Inverse der Modell-Transformationsmatrix

        Ray localRay;
        localRay.origin = invTransform * ray.origin;
        localRay.direction = invTransform * ray.direction; // Für Vektoren wird die translation ignoriert
        localRay.direction.normalize(); // Normalisiere den transformierten Richtungsvektor

        for (size_t triIdx = 0; triIdx < currentModel.mTriangles.size(); ++triIdx) {
            Triangle& currentTriangle = currentModel.mTriangles[triIdx];
            HitRecord tempHitRecord; // Temporärer HitRecord für jeden Schnitttest

            // Führe den Schnitttest im lokalen Raum durch
            if (triangleIntersect(localRay, currentTriangle, tempHitRecord, epsilon)) {
                // Wenn ein Schnitt im lokalen Raum gefunden wurde, transformiere den Schnittpunkt und die Normale zurück
                GLPoint worldIntersectionPoint = currentModel.mTransform * tempHitRecord.intersectionPoint;
                GLVector worldNormal = currentModel.mTransform * tempHitRecord.normal;
                worldNormal.normalize(); // Normale nach Transformation normalisieren

                // Berechne den neuen Parameter im Weltkoordinatensystem
                double dist = (worldIntersectionPoint - ray.origin).norm();
                // Vergleiche mit dem bisher besten Treffer
                if (dist < hitRecord.parameter) {
                    hit = true;
                    hitRecord.parameter = dist;
                    hitRecord.intersectionPoint = worldIntersectionPoint;
                    hitRecord.normal = worldNormal;
                    hitRecord.triangleId = triIdx;
                    hitRecord.modelId = modelIdx;
                    hitRecord.sphereId = -1; // Kein Kugel-Treffer
                    hitRecord.color = currentModel.getMaterial().color; // Materialfarbe vom Modell holen
                }
            }
        }
    }

    // Iteriere über alle Kugeln
    for (size_t sphereIdx = 0; sphereIdx < mSpheres.size(); ++sphereIdx) {
        Sphere& currentSphere = mSpheres[sphereIdx];
        HitRecord tempHitRecord; // Temporärer HitRecord

        if (sphereIntersect(ray, currentSphere, tempHitRecord, epsilon)) {
            // Vergleiche mit dem bisher besten Treffer
            if (tempHitRecord.parameter < hitRecord.parameter) {
                hit = true;
                hitRecord.parameter = tempHitRecord.parameter;
                hitRecord.intersectionPoint = tempHitRecord.intersectionPoint;
                hitRecord.normal = tempHitRecord.normal;
                hitRecord.triangleId = -1; // Kein Dreiecks-Treffer
                hitRecord.modelId = -1;   // Kein Modell-Treffer
                hitRecord.sphereId = sphereIdx;
                hitRecord.color = currentSphere.getMaterial().color; // Materialfarbe von der Kugel holen
            }
        }
    }
    return hit;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 * Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 * Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    // Möller-Trumbore Algorithmus
    const GLPoint& v0 = triangle.vertex[0];
    const GLPoint& v1 = triangle.vertex[1];
    const GLPoint& v2 = triangle.vertex[2];

    GLVector edge1 = v1 - v0;
    GLVector edge2 = v2 - v0;

    GLVector pvec = crossProduct(ray.direction, edge2);
    double det = dotProduct(edge1, pvec);

    // Wenn die Determinante nahe Null ist, ist der Strahl parallel zur Ebene des Dreiecks
    if (std::abs(det) < epsilon) {
        return false;
    }

    double invDet = 1.0 / det;

    GLVector tvec = ray.origin - v0;
    double u = dotProduct(tvec, pvec) * invDet;

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    GLVector qvec = crossProduct(ray.direction, edge1);
    double v = dotProduct(tvec, qvec) * invDet;

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = dotProduct(edge2, qvec) * invDet;

    // Strahl trifft Dreieck
    if (t > epsilon) { // t muss positiv sein (vor dem Ursprung des Strahls)
        hitRecord.parameter = t;
        hitRecord.intersectionPoint = ray.origin + ray.direction * t;
        hitRecord.normal = triangle.normal; // Normale des Dreiecks
        hitRecord.rayDirection = ray.direction;
        return true;
    }

    return false;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 * Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 * Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    GLVector oc = ray.origin - sphere.getPosition();
    double a = dotProduct(ray.direction, ray.direction);
    double b = 2.0 * dotProduct(oc, ray.direction);
    double c = dotProduct(oc, oc) - sphere.getRadius() * sphere.getRadius();

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // Kein Schnitt
    } else {
        double t0 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        double t = -1.0;
        if (t0 > epsilon) { // Prüfe den ersten Schnittpunkt
            t = t0;
        } else if (t1 > epsilon) { // Prüfe den zweiten Schnittpunkt, falls t0 negativ ist (Kamera in Kugel)
            t = t1;
        }

        if (t > epsilon) {
            hitRecord.parameter = t;
            hitRecord.intersectionPoint = ray.origin + ray.direction * t;
            hitRecord.normal = hitRecord.intersectionPoint - sphere.getPosition();
            hitRecord.normal.normalize();
            hitRecord.rayDirection = ray.direction;
            return true;
        }
    }
    return false;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) { mCamera = cam; }

std::shared_ptr<Camera> Scene::getCamera() const { return mCamera; }

GLPoint Scene::getViewPoint() const {
  if (mCamera)
    return mCamera->getEyePoint();
  else {
    std::cerr << "No Camera set to get view point from." << std::endl;
    return GLPoint(0, 0, 0);
  }
}

void Scene::addPointLight(GLPoint pointLight) {
  mPointLights.push_back(pointLight);
}

void Scene::addModel(Model model) { mModels.push_back(model); }

void Scene::addSphere(Sphere sphere) { mSpheres.push_back(sphere); }

std::vector<Model> &Scene::getModels() { return mModels; }
std::vector<Sphere> &Scene::getSpheres() { return mSpheres; }
std::vector<GLPoint> &Scene::getPointLights() { return mPointLights; }