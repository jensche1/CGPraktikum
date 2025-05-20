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

//void drawWindrad(WireframeRenderer& renderer, GLPoint point, Color baseColor);

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

 /** GLPoint zielpunkt(520, 340, 0);  // Linie gerade nach oben
  Color farbe(1.0, 0.0, 0.0);      // Startfarbe (rot)

  drawWindrad(wireRenderer, zielpunkt, farbe);**/



  /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier */

  /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier */

  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */

  auto& models = scene->getModels(); // Zugriff auf die Modelle, um Transformationen anzuwenden
  if (models.size() >= 2) {
      // Cube-Modell
      models[1].setTranslation(GLVector(150, 200, 0));
      models[1].setRotation(GLVector(20, 45, 0));
      models[1].setScale(GLVector(0.5, 3, 0.5));

      // Bunny-Modell
      models[0].setTranslation(GLVector(400, 200, 0));
      models[0].setRotation(GLVector(0, 5, 0));
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

  //  auto cam = std::make_shared<Camera>();
  //GLPoint eye = GLPoint(0.0, 0.0, 300.0);
  //cam->setEyePoint(eye);
  //cam->setUp(GLVector(0.0, 1.0, 0.0));
  //GLVector viewDirection = GLVector(0.0, 0, -1.0);
  //viewDirection.normalize();
  //cam->setViewDirection(viewDirection);
  //cam->setSize(img->getWidth(), img->getHeight());
  //scene->setCamera(cam);


  /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fŸgen Sie diese zur Szene hinzu*/

  /* Aufgabenblatt 4: Setzen Sie materialeigenschaften fŸr die Kugelen und die Modelle. Die Materialeigenschaften fŸr eine Darstellung entsprechend der Beispiellšsung ist in der Aufgabenstellung gegeben. */

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

/** void drawWindrad(WireframeRenderer& renderer, GLPoint point, Color baseColor) {
  // Mittelpunkt
  int cx = 320;
  int cy = 240;

  // Vektor vom Mittelpunkt zum Punkt
  double dx = point(0) - cx;
  double dy = point(1) - cy;

  double length = std::sqrt(dx * dx + dy * dy);

  // Ursprungswinkel
  double angle = std::atan2(dy, dx);  // in Radiant

  // 8 Farben fŸr die Linien
  std::vector<Color> colors = {
    Color(1.0, 0.0, 0.0),   // rot
    Color(1.0, 0.5, 0.0),   // orange
    Color(1.0, 1.0, 0.0),   // gelb
    Color(0.5, 1.0, 0.0),   // gelbgrŸn
    Color(0.0, 1.0, 0.0),   // grŸn
    Color(0.0, 1.0, 1.0),   // tŸrkis
    Color(0.0, 0.0, 1.0),   // blau
    Color(1.0, 0.0, 1.0)    // violett
}; **/

 /** for (int i = 0; i < 8; ++i) {
    double a = angle + i * M_PI / 4;  // 45¡ Schritte in Radiant
    int x = static_cast<int>(cx + length * std::cos(a));
    int y = static_cast<int>(cy + length * std::sin(a));

//Punkt auf 3/4 der nŠchsten Linie
    double b = angle + (i+1) * M_PI / 4;
    int xMid = static_cast<int>(cx + 0.75 * length * std::cos(b));
    int yMid = static_cast<int>(cy + 0.75 * length * std::sin(b));

    renderer.drawBresenhamLine(GLPoint(cx, cy, 0), GLPoint(x, y, 0), Color(0.0, 0.0, 0.0)); //Verbindung mit Mitte
    renderer.drawBresenhamLine(GLPoint (x,y,0), GLPoint (xMid, yMid, 0), Color(0.0, 0.0, 0.0)); // Verbindung mit nŠchster Linie

  } **/


  /** for (int i = 0; i < 8; ++i) {
    double a = angle + i * M_PI / 4;  // 45¡ Schritte in Radiant
    int x = static_cast<int>(cx + length * std::cos(a));
    int y = static_cast<int>(cy + length * std::sin(a));

    // Berechne ungefŠhren Mittelpunkt des Dreiecks
    int sx = (cx + x + xMid) / 3;
    int sy = (cy + y + yMid) / 3;
    GLPoint seed(sx, sy, 0);

    // FŸlle das Dreieck mit der Farbe
    renderer.seedFillArea(seed, Color(0.0, 0.0, 0.0), colors[i]); // schwarz als Randfarbe

  }


} **/