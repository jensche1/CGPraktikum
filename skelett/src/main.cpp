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
#include <random>

#include "Color.hpp"
#include "GLPoint.hpp"
#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"


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

  /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier */

  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */

  auto& models = scene->getModels(); // Zugriff auf die Modelle, um Transformationen anzuwenden
  if (models.size() >= 5) {
      // Cube-Modell 1
      models[1].setTranslation(GLVector(150, 200, 0));
      models[1].setRotation(GLVector(20, 45, 0));
      models[1].setScale(GLVector(0.5, 3, 0.5));
      //Cube-Modell 2
      models[2].setTranslation(GLVector(-60,-50,0));
      models[2].setRotation(GLVector(0, 0, 0));
      models[2].setScale(GLVector(0.1, 2, 0.3));
      //Cube-Modell 3
      models[3].setTranslation(GLVector(60,50,-50));
      models[3].setRotation(GLVector(0, 0, 0));
      models[3].setScale(GLVector(2, 1, 0.5));
      //Cube-Modell 4
      models[4].setTranslation(GLVector(-80,10,-100));
      models[4].setRotation(GLVector(0, 0, 0));
      models[4].setScale(GLVector(0.3, 1.3, 3));
      // Bunny-Modell
      models[0].setTranslation(GLVector(0, -110, -30));
      models[0].setRotation(GLVector(0, 175, 0));
      models[0].setScale(GLVector(2, 2, 2));

  }


  /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */

  wireRenderer.renderScene(Color(0.0, 0.0, 1.0));  // Zeichnet Szene in Blau





  /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
  // Diese Einstellungen beziehen sich auf den world space
  // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
  // den ViewSpace benštigen, da die Strahen fŸr Raycasting und Raytracing im
  // World space definiert sind. Modelle mŸssen also lediglich in den World
  // space ŸberfŸhrt werden

  /* Aufgabenblatt 3:  kommentieren Sie die Zeilen wieder ein, die eine Kamera erzeugen und zur Scene hinzufŸgen*/

  auto cam = std::make_shared<Camera>();
  GLPoint eye = GLPoint(0.0, 0.0, 200.0);
  cam->setEyePoint(eye);
  cam->setUp(GLVector(0.0, 1.0, 0.0));
  GLVector viewDirection = GLVector(0.0, 0, -1.0);
  viewDirection.normalize();
  cam->setViewDirection(viewDirection);
  cam->setSize(img->getWidth(), img->getHeight());
  scene->setCamera(cam); 


  /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fuegen Sie diese zur Szene hinzu*/
  Sphere sphere1;
  sphere1.setPosition(GLPoint(150,0,30));
  sphere1.setRadius(50.0);

  /* Aufgabenblatt 4: Setzen Sie materialeigenschaften fŸr die Kugelen und die Modelle. Die Materialeigenschaften fŸr eine Darstellung entsprechend der Beispiellšsung ist in der Aufgabenstellung gegeben. */
  Material redMaterial;
  redMaterial.color = Color(1.0, 0.0, 0.0);
  redMaterial.smooth = 0.5;
  redMaterial.reflection = 0.3;
  redMaterial.refraction = 0.0;
  redMaterial.transparency = 0.0;
  sphere1.setMaterial(redMaterial);

  scene->addSphere(sphere1);

  /* Aufgabenblatt 3: (Wenn nštig) Transformationen der Modelle im World space, sodass sie von der Kamera gesehen werden kšnnnen. Die nštigen Transformationen fŸr eine Darstellung entsprechend der Beispiellšsung ist in der Aufgabenstellung gegeben. */

  /* Stelle materialeigenschaften zur verfŸgung (Relevant fŸr Aufgabenblatt 4)*/

  /* Aufgabenblatt 4  FŸgen Sie ein Licht zur Szene hinzu */

  /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mir einem shared_ptr) und rufen sie die Funktion renderRaycast auf */

  // Schreiben des Bildes in Datei
  if (argc > 1) {
    img->writeAsPPM(argv[1]);
    std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
              << img->getHeight() << " in Datei " << argv[1] << " geschrieben."
              << std::endl;
  } else {
    std::cerr
        << "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
        << std::endl;
  }

  return 0;
}