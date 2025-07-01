#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include <limits>

Scene::Scene() {
}

/**
 * Lädt 3D-Modelle aus den angegebenen Dateien und fügt sie der Szene hinzu.
 * (Aufgabenblatt 1, Aufgabe 1)
 */
void Scene::load(const std::vector<std::string> &pFiles) {
    std::cout << "Laden der PLY Dateien:" << std::endl;
    // Für alle Objekte (Objekt meint hier das selbe wie Model)
    for (int obj_nr = 0; obj_nr < pFiles.size(); obj_nr++) {
        std::cout << "\tModel-Index: " << obj_nr << std::endl;
        // Assimp übernimmt das Einlesen der ply-Dateien
        Assimp::Importer importer;
        const aiScene *assimpScene = importer.ReadFile(
                pFiles[obj_nr], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices |
                                aiProcess_SortByPType);

        if (!assimpScene) {
            std::cout << importer.GetErrorString() << std::endl;
        }
        assert(assimpScene);
        auto meshes = assimpScene->mMeshes;
        // Neues Model erstellen
        Model model = Model();

        // Für alle Meshes des Models
        for (int i = 0; i < assimpScene->mNumMeshes; i++) {
            std::cout << "\t\tMesh-Index: " << i << " (" << meshes[i]->mNumFaces
                      << " Faces)" << std::endl;
            auto faces = meshes[i]->mFaces;
            auto vertices = meshes[i]->mVertices;

            // Für alle Faces einzelner Meshes
            for (int j = 0; j < meshes[i]->mNumFaces; j++) {
                // Dreieck konstruieren und nötige Werte berechnen
                Triangle tri;
                assert(faces[j].mNumIndices == 3);
                tri.vertex[0] = GLPoint(vertices[faces[j].mIndices[0]].x,
                                        vertices[faces[j].mIndices[0]].y,
                                        vertices[faces[j].mIndices[0]].z);
                tri.vertex[1] = GLPoint(vertices[faces[j].mIndices[1]].x,
                                        vertices[faces[j].mIndices[1]].y,
                                        vertices[faces[j].mIndices[1]].z);
                tri.vertex[2] = GLPoint(vertices[faces[j].mIndices[2]].x,
                                        vertices[faces[j].mIndices[2]].y,
                                        vertices[faces[j].mIndices[2]].z);
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

    std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 * (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
     // Initialisiere mit maximalem Wert
    bool hit_Anything = false;
    double t;
    GLVector normal;

    for (size_t i = 0; i < getModels().size() ; i++) { // i= 0 ist bunny
        int j = 0;
        for (Triangle triangle : getModels()[i].mTriangles) {             //  Fehlersuche: hier war ein Fehler im Aufruf der Triangles
            GLMatrix transformation = getModels()[i].getTransformation();
            GLPoint p_1 = transformation*triangle.vertex[0];
            GLPoint p_2 = transformation*triangle.vertex[1];
            GLPoint p_3 = transformation*triangle.vertex[2];
            Triangle transformiertes_triangle;
            transformiertes_triangle.vertex[0] =p_1;
            transformiertes_triangle.vertex[1] =p_2;
            transformiertes_triangle.vertex[2] =p_3;
            if (triangleIntersect(ray,transformiertes_triangle,hitRecord,epsilon)) {
                hitRecord.modelId = i;
                hitRecord.triangleId = j;
                hitRecord.sphereId=-1;
                //hitRecord.color = getModels()[i].getMaterial().color;
                hit_Anything = true;
            }
            j++;
        }
    }

    for (size_t i = 0; i < getSpheres().size(); i++) {
        if (sphereIntersect(ray, getSpheres()[i], hitRecord, epsilon)) {
            hitRecord.sphereId = i;
            hitRecord.modelId = -1;
            hitRecord.triangleId = -1;
            //hitRecord.color = getSpheres()[i].getMaterial().color;
            hit_Anything = true;
        }
    }
    return hit_Anything;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 * Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 * Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
//4.2
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {

    GLPoint p1 = triangle.vertex[0];
    GLPoint p2 = triangle.vertex[1];
    GLPoint p3 = triangle.vertex[2];

    GLVector n = crossProduct(p2 - p1, p3 - p1);

    // Prüfe, ob der Strahl parallel zur Dreiecksebene ist
    double n_dot_raydir = dotProduct(n, ray.direction);
    if (std::abs(n_dot_raydir) < epsilon) {
        return false;
    }

    // Berechne die Distanz t zum Schnittpunkt
    double t = dotProduct(p1 - ray.origin, n) / n_dot_raydir;

    // Liegt der Schnittpunkt hinter dem Strahl oder ist er weiter weg als der bisher nächste?
    if (t < epsilon || t >= hitRecord.parameter) {
        return false;
    }

    // Berechne den Schnittpunkt
    GLPoint p = ray.origin + t * ray.direction;

    // Schutz vor Division durch Null bei degenerierten Dreiecken (Fläche = 0)
    double flaecheABC = dotProduct(n, n);
    if (flaecheABC < epsilon) {
        return false;
    }

    // Baryzentrische Koordinaten berechnen, um zu prüfen, ob der Punkt im Dreieck liegt
    // (Diese Methode ist numerisch stabiler)
    GLVector edge1 = p2 - p1;
    GLVector edge2 = p3 - p1;
    GLVector p_vec = p - p1;

    double d00 = dotProduct(edge1, edge1);
    double d01 = dotProduct(edge1, edge2);
    double d11 = dotProduct(edge2, edge2);
    double d20 = dotProduct(p_vec, edge1);
    double d21 = dotProduct(p_vec, edge2);

    double denom = d00 * d11 - d01 * d01;
    if (std::abs(denom) < epsilon) { // Schutz vor Division durch Null
        return false;
    }

    double beta = (d11 * d20 - d01 * d21) / denom;
    double gamma = (d00 * d21 - d01 * d20) / denom;

    if (beta < 0.0 || gamma < 0.0 || (beta + gamma) > 1.0) {
        return false;
    }

    // Ein gültiger Treffer wurde gefunden!
    hitRecord.parameter = t;
    hitRecord.intersectionPoint = p;

    // Normale für die Beleuchtung normalisieren und speichern
    n.normalize();
    hitRecord.normal = n;

    return true;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 * Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 * Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    GLPoint e = ray.origin;
    GLVector v = ray.direction;
    GLPoint m = sphere.getPosition();
    double r = sphere.getRadius();

    double a = dotProduct(v,v);
    double b = 2 * dotProduct(v,e-m);
    double c = dotProduct(e - m,e - m) - r*r;

    double diskr = b*b - 4*a*c;

    if (diskr < 0) {
        return false;
    }

    double t = (-b - sqrt(diskr)) / (2*a);

    //mit Parameter aus hitrecord abgleichen
    if (hitRecord.parameter > t) {
        if(t > epsilon) {
            hitRecord.parameter = t;
            hitRecord.intersectionPoint = ray.origin + t * ray.direction;
            //  4.2
            GLVector normal = hitRecord.intersectionPoint - sphere.getPosition();
            normal.normalize();
            hitRecord.normal = normal;
            //  4.2

            return true;
        }
    }

    t = (-b + sqrt(diskr)) / (2*a);
    //mit Parameter aus hitrecord abgleichen
    if (hitRecord.parameter > t) {
        if(t > epsilon) {
            hitRecord.parameter = t;
            hitRecord.intersectionPoint = ray.origin + t * ray.direction;
            //  4.2
            GLVector normal = hitRecord.intersectionPoint - sphere.getPosition();
            normal.normalize();
            hitRecord.normal = normal;
            // 4.2
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