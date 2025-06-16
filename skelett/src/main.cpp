// main.cpp Anpassungen
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <complex>
#include <unistd.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <random> // Für mögliche Antialiasing-Implementierung

#include "Color.hpp"
#include "GLPoint.hpp"
#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"
#include "Image.hpp" // Sicherstellen, dass Image inkludiert ist

int main(int argc, char **argv) {
    // Dimensionen des Ergebnisbildes im Konstruktor setzen
    std::shared_ptr<Image> img = std::make_shared<Image>(640, 480);

    // Verwendete Modelle festlegen
    std::vector<std::string> path_vector;
    path_vector.push_back(std::string("../data/bunny/bunny_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
    auto scene = std::make_shared<Scene>();
    scene->load(path_vector);

    /* Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer */
    auto wireRenderer = WireframeRenderer(scene, img);

    /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier */

    /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre drawTriangle-Methode hier */

    /* Aufgabenblatt 2: Testen Sie Ihre drawFilledTriangle-Methode hier */

    /* Aufgabenblatt 2: Testen Sie Ihre renderScene-Methode hier (Wireframe) */

    // Hier Kamera Code einkommentieren und Parameter setzen (Aufgabenblatt 3)
    // Kameraposition: [0,0,200]
    // Blickrichtung: standardmäßig (0,0,-1)
    // Up-Vektor: standardmäßig (0,1,0)
    auto cam = std::make_shared<Camera>();
    cam->setEyePoint(GLPoint(0, 0, 200));
    cam->setViewDirection(GLVector(0, 0, -1));
    cam->setUp(GLVector(0, 1, 0));
    cam->setSize(img->getWidth(), img->getHeight());
    cam->update();
    scene->setCamera(cam);

    // Bunny Transformationen (Aufgabenblatt 3)
    // Verschiebung um -10 Einheiten in y-Richtung und -30 Einheiten in z-Richtung
    // Rotation um 170 Grad um y-Achse
    Model& bunnyModel = scene->getModels()[0]; // Annahme: Bunny ist das erste geladene Modell
    bunnyModel.setTranslation(GLVector(0, -10, -30));
    bunnyModel.setRotation(GLVector(0, 170, 0));
    // Bunny Materialfarbe: [0,1,0]
    Material bunnyMaterial;
    bunnyMaterial.color = Color(0.0, 1.0, 0.0);
    /


    bunnyModel.mMaterial = bunnyMaterial;

    Model& cube1 = scene->getModels()[1]; // Annahme: Cube ist das zweite geladene Modell
    cube1.setTranslation(GLVector(-60, -50, 0));
    Material matCube1;
    matCube1.color = Color(0.9, 0.9, 0.3);
    cube1.mMaterial = matCube1;


    Model cube2 = scene->getModels()[1];
    cube2.setTranslation(GLVector(60, 50, -50));
    Material matCube2;
    matCube2.color = Color(0.3, 0.9, 0.9);
    cube2.mMaterial = matCube2;
    scene->addModel(cube2);

    // Cube 3: [-80, 10, -100], Material [0.9, 0.3, 0.9]
    Model cube3 = scene->getModels()[1]; // Kopie
    cube3.setTranslation(GLVector(-80, 10, -100));
    Material matCube3;
    matCube3.color = Color(0.9, 0.3, 0.9);
    cube3.mMaterial = matCube3;
    scene->addModel(cube3);

    // Cube 4: [0, -100, 0], Skalierung [500, 0.01, 500], Material [0.5, 0.5, 0.5] (Boden)
    Model cube4 = scene->getModels()[1]; // Kopie
    cube4.setTranslation(GLVector(0, -100, 0));
    cube4.setScale(GLVector(500, 0.01, 500));
    Material matCube4;
    matCube4.color = Color(0.5, 0.5, 0.5);
    cube4.mMaterial = matCube4;
    scene->addModel(cube4);


    // Spheres (Aufgabenblatt 3)
    // Sphere 1: [150, 0, 30], Radius 50.0, Material [1.0, 0.0, 0.0] (bereits teilweise im Snippet)
    Sphere sphere1;
    sphere1.setPosition(GLPoint(150, 0, 30));
    sphere1.setRadius(50.0);
    Material redMaterial;
    redMaterial.color = Color(1.0, 0.0, 0.0);
    redMaterial.smooth = 0.5; // Diese Werte sind für Aufgabenblatt 4, aber hier schon gesetzt
    redMaterial.reflection = 0.3;
    redMaterial.refraction = 0.0;
    redMaterial.transparency = 0.0;
    sphere1.setMaterial(redMaterial);
    scene->addSphere(sphere1);

    // Sphere 2: [-150, 20, 0], Radius 30.0, Material [0.0, 0.0, 1.0]
    Sphere sphere2;
    sphere2.setPosition(GLPoint(-150, 20, 0));
    sphere2.setRadius(30.0);
    Material blueMaterial;
    blueMaterial.color = Color(0.0, 0.0, 1.0);
    blueMaterial.smooth = 0.5;
    blueMaterial.reflection = 0.3;
    blueMaterial.refraction = 0.0;
    blueMaterial.transparency = 0.0;
    sphere2.setMaterial(blueMaterial);
    scene->addSphere(sphere2);


    /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mir einem shared_ptr) und rufen sie die Funktion renderRaycast auf */
    auto solidRenderer = std::make_shared<SolidRenderer>(scene, img, cam);
    solidRenderer->renderRaycast();

    // Schreiben des Bildes in Datei
    if (argc > 1) {
        img->writeAsPPM(argv[1]);
        std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                  << img->getHeight() << " in Datei " << argv[1]
                  << " geschrieben." << std::endl;
    } else {
        std::cout << "Kein Dateiname angegeben. Bild wird nicht gespeichert."
                  << std::endl;
    }

    return 0;
}