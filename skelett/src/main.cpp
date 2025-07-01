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

    //  4.2
    // Lichtquelle hinzufügen, wie im Aufgabenblatt vorgeschlagen
    scene->addPointLight(GLPoint(0, 150, 150));
    //  4.2

    Model &cube = scene->getModels()[1];
    //cube kopien
    Model cube_2 = cube;
    Model cube_3 = cube;
    Model cube_4 = cube;

    scene->addModel(cube_2);
    scene->addModel(cube_3);
    scene->addModel(cube_4);

    /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fügen Sie diese zur Szene hinzu*/
    Sphere sphere01;
    scene->addSphere(sphere01);
    Sphere sphere02;
    scene->addSphere(sphere02);

    Sphere &sphere1 = scene->getSpheres()[0];
    sphere1.setRadius(50);
    sphere1.setPosition(GLPoint(-150, 0.0, -30));

    Sphere &sphere2 = scene->getSpheres()[1];
    sphere2.setRadius(50);
    sphere2.setPosition(GLPoint(150, 30.0, -30));

    /* Problemsuche: Modelle noch einmal neu geladen, jetzt sind Würfel aufgetaucht */
    Model &cube1 = scene->getModels()[1];
    cube1.setTranslation(GLVector(-60, -50, 0));
    cube1.setScale(GLVector(1, 1, 1));

    Model &cube2 = scene->getModels()[2];
    cube2.setTranslation(GLVector(60, 50, -50));
    cube2.setScale(GLVector(1, 1, 1));


    Model &cube4 = scene->getModels()[4];
    cube4.setTranslation(GLVector(0, -100, 0));
    cube4.setScale(GLVector(500, 0.01, 500));

    Model &cube3 = scene->getModels()[3];
    cube3.setTranslation(GLVector(-80, 10, -100));
    cube3.setScale(GLVector(1, 1, 1));


    Model &bunny = scene->getModels()[0];
    bunny.setTranslation(GLVector(0, -10, -30));
    bunny.setRotation(GLVector(0, 170,0));
    bunny.setScale(GLVector(1, 1, 1));

    // 4.1
    bunny.setScale(GLVector(1, 1, 1));

    // Aufgabe 1: "Raum" als umschließenden Würfel hinzufügen
    Model room_cube_copy = scene->getModels()[1]; // Eine Kopie des Würfel-Modells erstellen
    scene->addModel(room_cube_copy); // Den neuen Würfel zur Szene hinzufügen

    // Eine Referenz auf den soeben hinzugefügten Würfel holen (er ist das letzte Element)
    Model &room = scene->getModels().back();

    // Den Würfel so skalieren und positionieren, dass er die Szene umschließt
    // und ihm ein Material zuweisen, gemäß Aufgabenblatt.
    room.setTranslation(GLVector(0, 100, 0));
    room.setScale(GLVector(500, 500, 500));

    Material room_material;
    room_material.color = Color(0.99, 0.99, 0.99);
    room.setMaterial(room_material);

    // 4.1

    /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/
    Material cube1_material;
    cube1_material.color = Color(0.9, 0.9, 0.3);
    // 4.2
    cube1_material.ambient = Color(0.1, 0.1, 0.05);
    cube1_material.diffuse = Color(0.9, 0.9, 0.3);
    cube1_material.specular = Color(0.5, 0.5, 0.5);
    cube1_material.shininess = 32.0f;
    // 4.2
    Material cube2_material;
    cube2_material.color = Color(0.9, 0.4, 0.3);
// 4.2
    cube2_material.ambient = Color(0.1, 0.05, 0.05);
    cube2_material.diffuse = Color(0.9, 0.4, 0.3);
    cube2_material.specular = Color(0.7, 0.7, 0.7);
    cube2_material.shininess = 64.0f;
    //4.2
    Material cube3_material;
    cube3_material.color = Color(1.0, 0, 0); // 0.1 von Aufgabenblatt stimmt nicht mit Zielbild
// 4.2
    cube3_material.ambient = Color(0.2, 0.0, 0.0);
    cube3_material.diffuse = Color(1.0, 0.0, 0.0);
    cube3_material.specular = Color(0.8, 0.8, 0.8);
    cube3_material.shininess = 128.0f;
    // 4.2
    Material cube4_material;
    cube4_material.color = Color(0.9, 0.9, 0.9);
    cube4_material.ambient = Color(0.2, 0.2, 0.2);
    cube4_material.diffuse = Color(0.9, 0.9, 0.9);
    cube4_material.specular = Color(0.1, 0.1, 0.1); // Kaum glänzend
    cube4_material.shininess = 10.0f;

    Material materialsphere1 = Material();
    // 4.4
    materialsphere1.color = Color(0.0, 0.0, 0.0); // <-- Auf Schwarz setzen
    materialsphere1.ambient = Color(0.0, 0.0, 0.0);
    materialsphere1.diffuse = Color(0.0, 0.0, 0.0);
    materialsphere1.specular = Color(1.0, 1.0, 1.0); // Glanzlicht bleibt für den "spiegelnden" Effekt
    materialsphere1.shininess = 512.0f;
    materialsphere1.reflection = 1.0f; // <-- Ist bereits 1.0


    Material materialsphere2 = Material();
    // 4.4
    materialsphere2.color = Color(0.0, 0.0, 0.0); // <-- Auf Schwarz setzen
    materialsphere2.ambient = Color(0.0, 0.0, 0.0);
    materialsphere2.diffuse = Color(0.0, 0.0, 0.0);
    materialsphere2.specular = Color(1.0, 1.0, 1.0);
    materialsphere2.shininess = 512.0f;
    materialsphere2.reflection = 1.0f;

    Material materialbunny = Material();
    // 4.4
    materialbunny.color = Color(0.0, 0.0, 0.0); // <-- Auf Schwarz setzen
    materialbunny.ambient = Color(0.0, 0.0, 0.0);
    materialbunny.diffuse = Color(0.0, 0.0, 0.0);
    materialbunny.specular = Color(1.0, 1.0, 1.0);
    materialbunny.shininess = 512.0f;
    materialbunny.reflection = 1.0f;



    bunny.setMaterial(materialbunny);
    cube1.setMaterial(cube1_material);
    cube2.setMaterial(cube2_material);
    cube3.setMaterial(cube3_material);
    cube4.setMaterial(cube4_material); // Wichtig: Auch dem Boden das Material zuweisen
    sphere1.setMaterial(materialsphere1);
    sphere2.setMaterial(materialsphere2);
    scene->getSpheres()[0].setMaterial(materialsphere1);
    scene->getSpheres()[1].setMaterial(materialsphere2);


    /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mir einem shared_ptr) und rufen sie die Funktion renderRaycast auf */
    SolidRenderer solidRenderer(scene, img, cam);
    solidRenderer.renderRaycast();

    // Schreiben des Bildes in Datei
    /*if (argc > 1) {
        img->writeAsPPM(argv[1]);
        std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                  << img->getHeight() << " in Datei " << argv[1]
                  << " geschrieben." << std::endl;
    } else {
        std::cout << "Kein Dateiname angegeben. Bild wird nicht gespeichert."
                  << std::endl;
    }

    return 0;*/
    if (argc > 1) {
        img->writeAsPPM(argv[1]);
        std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                  << img->getHeight() << " in Datei " << argv[1]
                  << " geschrieben." << std::endl;
        system(("open " + std::string(argv[1])).c_str()); // Mac: Bild anzeigen
    } else {
        std::string defaultFilename = "output.ppm";
        img->writeAsPPM(defaultFilename);
        std::cout << "Kein Dateiname angegeben. Bild wird als " << defaultFilename << " gespeichert."
                  << std::endl;
        system(("open " + defaultFilename).c_str()); // Mac: Bild anzeigen
    }
}