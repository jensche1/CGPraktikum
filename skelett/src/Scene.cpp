#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath> // For std::sqrt

Scene::Scene() {
}


/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 * (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    bool hit_Anything = false;
    // Reset hitRecord parameter for new intersection test
    hitRecord.parameter = std::numeric_limits<double>::infinity();
    hitRecord.modelId = -1;
    hitRecord.sphereId = -1;


    for (size_t i = 0; i < getModels().size() ; i++) { // i= 0 ist bunny
        for (Triangle triangle : getModels()[i].mTriangles) {
            GLMatrix transformation = getModels()[i].getTransformation();
            GLPoint p_1 = transformation*triangle.vertex[0];
            GLPoint p_2 = transformation*triangle.vertex[1];
            GLPoint p_3 = transformation*triangle.vertex[2];
            Triangle transformiertes_triangle;
            transformiertes_triangle.vertex[0] =p_1;
            transformiertes_triangle.vertex[1] =p_2;
            transformiertes_triangle.vertex[2] =p_3;
            // Temporärer HitRecord für den aktuellen Treffertest
            HitRecord current_hit;
            current_hit.parameter = hitRecord.parameter; // Wichtig: Aktuellen Bestwert übergeben
            current_hit.rayDirection = ray.direction; // Setze die Strahlenrichtung für das Shading


            if (triangleIntersect(ray,transformiertes_triangle,current_hit,epsilon)) {
                if (current_hit.parameter < hitRecord.parameter) { // Nur wenn dieser Treffer näher ist
                    hitRecord = current_hit;
                    hitRecord.modelId = i;
                    hitRecord.color = getModels()[i].getMaterial().color;
                    hit_Anything = true;
                }
            }
        }
    }
    for (size_t i = 0; i < getSpheres().size(); i++) {
        // Temporärer HitRecord für den aktuellen Treffertest
        HitRecord current_hit;
        current_hit.parameter = hitRecord.parameter; // Wichtig: Aktuellen Bestwert übergeben
        current_hit.rayDirection = ray.direction; // Setze die Strahlenrichtung für das Shading


        if (sphereIntersect(ray, getSpheres()[i], current_hit, epsilon)) {
            if (current_hit.parameter < hitRecord.parameter) { // Nur wenn dieser Treffer näher ist
                hitRecord = current_hit;
                hitRecord.sphereId = i;
                hitRecord.color = getSpheres()[i].getMaterial().color;
                hit_Anything = true;
            }
        }
    }


    return hit_Anything;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 * Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 * Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    GLVector edge1, edge2, h, s, q;
    double a, f, u, v;

    edge1 = triangle.vertex[1] - triangle.vertex[0];
    edge2 = triangle.vertex[2] - triangle.vertex[0];

    h = crossProduct(ray.direction, edge2);
    a = dotProduct(edge1, h);

    if (a > -epsilon && a < epsilon)
        return false; // This ray is parallel to this triangle.

    f = 1.0 / a;
    s = ray.origin - triangle.vertex[0];
    u = f * dotProduct(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    q = crossProduct(s, edge1);
    v = f * dotProduct(ray.direction, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage, we can compute t to find out where the intersection point is on the line.
    double t = f * dotProduct(edge2, q);

    if (t > epsilon && t < hitRecord.parameter) // ray intersection
    {
        hitRecord.parameter = t;
        hitRecord.intersectionPoint = ray.origin + t * ray.direction;
        hitRecord.normal = triangle.normal; // For flat shading, use the triangle's normal
        hitRecord.rayDirection = ray.direction; // Store the incoming ray direction
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
    GLVector L = ray.origin - sphere.getPosition();
    double a = dotProduct(ray.direction, ray.direction);
    double b = 2.0 * dotProduct(ray.direction, L);
    double c = dotProduct(L, L) - sphere.getRadius() * sphere.getRadius();

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // No intersection
    }

    double t0, t1;
    if (discriminant == 0) {
        t0 = t1 = -b / (2 * a);
    } else {
        t0 = (-b - std::sqrt(discriminant)) / (2 * a);
        t1 = (-b + std::sqrt(discriminant)) / (2 * a);
    }

    // Find the smallest positive t that is greater than epsilon
    double t = std::numeric_limits<double>::infinity();
    if (t0 > epsilon && t0 < hitRecord.parameter) {
        t = t0;
    }
    if (t1 > epsilon && t1 < hitRecord.parameter) {
        if (t1 < t) { // Check if t1 is closer than t0 (if t0 was already set)
            t = t1;
        }
    }

    if (t == std::numeric_limits<double>::infinity()) {
        return false; // No valid intersection
    }

    hitRecord.parameter = t;
    hitRecord.intersectionPoint = ray.origin + t * ray.direction;
    // Corrected line: First assign the vector, then normalize it in place
    hitRecord.normal = (hitRecord.intersectionPoint - sphere.getPosition());
    hitRecord.normal.normalize(); // Normal for Phong shading
    hitRecord.rayDirection = ray.direction; // Store the incoming ray direction

    return true;
}


void Scene::load(const std::vector<std::string> &pFiles) {
    Assimp::Importer importer;

    for (const std::string &pFile : pFiles) {
        const aiScene *scene = importer.ReadFile(
            pFile, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                       aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString()
                      << std::endl;
            return;
        }

        // Alle Meshes in der Szene durchlaufen
        for (size_t i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[i];

            // Neues Model erstellen
            Model model;
            // Jedes Face (Dreieck) im Mesh durchlaufen
            for (size_t j = 0; j < mesh->mNumFaces; ++j) {
                aiFace face = mesh->mFaces[j];
                // Überprüfen, ob das Face ein Dreieck ist
                if (face.mNumIndices == 3) {
                    Triangle tri;
                    // Eckpunkte des Dreiecks speichern
                    for (size_t k = 0; k < 3; ++k) {
                        aiVector3D vertex = mesh->mVertices[face.mIndices[k]];
                        tri.vertex[k] = GLPoint(vertex.x, vertex.y, vertex.z);
                    }
                    // Normale des Dreiecks berechnen und speichern
                    GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                                   tri.vertex[2] - tri.vertex[0]);
                    normal.normalize();
                    tri.normal = normal;
                    // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
                    model.mTriangles.push_back(tri);
                }
            }
            // Immer das gleiche Material für das Model setzen
            Material material;
            material.color = Color(0.00, 1.00, 0.00);
            model.setMaterial(material);
            // Jedes Model zum Vector der Models der Scene hinzufügen
            mModels.push_back(model);
        }
    }

    std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
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