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
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord, const float epsilon) {
  bool hit = false;

  // Iteriere über alle Modelle (Dreiecke)
  for (size_t modelIdx = 0; modelIdx < mModels.size(); ++modelIdx) {
    Model& currentModel = mModels[modelIdx];
    // Transformation des Strahls in den lokalen Koordinatenraum des Modells
    // Ray_local.origin = M_inv * Ray_world.origin
    // Ray_local.direction = M_inv * Ray_world.direction (w=0 für Vektor)
    GLMatrix invTransform = currentModel.mTransform;

    for (size_t triIdx = 0; triIdx < currentModel.mTriangles.size(); ++triIdx) {
      Triangle& currentTriangle = currentModel.mTriangles[triIdx];

      Triangle newTriangle;

      newTriangle.vertex[0] = invTransform * currentTriangle.vertex[0];
      newTriangle.vertex[1] = invTransform * currentTriangle.vertex[1];
      newTriangle.vertex[2] = invTransform * currentTriangle.vertex[2];
      // Führe den Schnitttest im lokalen Raum durch
      if (triangleIntersect(ray,newTriangle, hitRecord, epsilon)) {
        hitRecord.modelId = modelIdx;
        hitRecord.triangleId = triIdx;
        hitRecord.sphereId = -1;
        hit = true; 
      }
    }
  }

    // Iteriere über alle Kugeln
  for (size_t sphereIdx = 0; sphereIdx < mSpheres.size(); ++sphereIdx) {
    Sphere& currentSphere = mSpheres[sphereIdx];

    if (sphereIntersect(ray, currentSphere, hitRecord, epsilon)) {
    // Vergleiche mit dem bisher besten Treffer
      hitRecord.modelId = -1;
      hitRecord.triangleId = -1;
      hitRecord.sphereId = sphereIdx;
      hit = true;
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
    if (t > epsilon && t < hitRecord.parameter) { // t muss positiv sein (vor dem Ursprung des Strahls)
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

        if (t > epsilon && t < hitRecord.parameter) {
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