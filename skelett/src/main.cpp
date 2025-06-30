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

//test
int main(int argc, char **argv) {
    // Dimensionen des Ergebnisbildes im Konstruktor setzen
    std::shared_ptr<Image> img = std::make_shared<Image>(1920, 1080);

    // Verwendete Modelle festlegen
    std::vector<std::string> path_vector;
    path_vector.push_back(std::string("../data/bunny/bunny_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
    auto scene = std::make_shared<Scene>();
    scene->load(path_vector);


    /* Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer */
    //WireframeRenderer wfr = WireframeRenderer(scene, img);
    Color black_color(0, 0, 0); //Linienfarbe
    /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier

    //Zentrum gewählt
        double ursprungX = img->getWidth() / 2.0;
        double ursprungY = img->getHeight() / 2.0;
        GLPoint ursprung(ursprungX, ursprungY, 0.0);

        //n+1 ist die Anzahl der Dreiecke, da 0 indiziert
        int n = 15;
        std::vector<unsigned int> anzahl_eckpunkte_vector;
        for (unsigned int i = 0; i <= n; i++)
        {
            anzahl_eckpunkte_vector.push_back(i);
        }

        int a = 200; //Skalierung
        std::vector<std::complex<double>> eckpunkte_vector;
        for (unsigned int eckpunkt : anzahl_eckpunkte_vector)
        {
            eckpunkte_vector.push_back(
                std::complex<double>(
                    a * cos(eckpunkt * (M_PI / 8)),
                    a * sin(eckpunkt * (M_PI / 8)))); // Vektor z_n
        }

        int anzahl = eckpunkte_vector.size();
        for (int i = 0; i < eckpunkte_vector.size(); i++)
        {
            GLPoint punkt_gl_point_1(
                eckpunkte_vector[(i + 1) % anzahl].real() + ursprung(0),
                eckpunkte_vector[(i + 1) % anzahl].imag() + ursprung(1),
                0.0); // Eckpunkt mit Verschiebung Richtung Zentrum
            GLPoint punkt_gl_point_2(0.9 * (eckpunkte_vector[i].real()) + ursprung(0),
                                     0.9 * (eckpunkte_vector[i].imag()) + ursprung(1),
                                     0.0); //Punkte auf etwa 90% der Geradenlänge

            wfr.drawBresenhamLine(ursprung, punkt_gl_point_1, black_color); //Gerade vom Ursprung
            wfr.drawBresenhamLine(punkt_gl_point_1, punkt_gl_point_2, black_color); // Gerade zur Verbindung
        }
*/
    /**     Punkte generieren Helene
                //komplexe Zahl c stellt den Mittelpunkt des Bildes im komplexen Raum dar – sie wird verwendet, um alle anderen Punkte (zₙ) relativ zu diesem Mittelpunkt zu platzieren (also verschoben auf den Bildschirmbereich).
                std::complex<double> ursprungComplex(ursprungX, ursprungY);
                //Eulersche Formel für die Endpunkte

                const double a = 200.0;
                const int N = 15;
                const double PI = 3.141592;
                std::vector<std::complex<double>> zn(N), pn(N), p0n(N);
                std::vector<GLPoint> pnPoints(N);
                std::vector<GLPoint> p0nPoints(N);

                //Punkte erstellen
                for (int n = 0; n < N; ++n)
                {
                    zn[n] = a * std::polar(1.0, (PI * n) / 8); //Eulerformel vorläufige Endpunkte
                    pn[n] = zn[n] + ursprungComplex; //Verschiebung der zn um c um endgültige Endpunkte zu erhalten
                    p0n[n] = (0.9 * (pn[n] - ursprungComplex)) + ursprungComplex; //Punkte auf 0,9 Höhe der Geraden

                    pnPoints[n] = GLPoint(pn[n].real(), pn[n].imag(), 0);
                    p0nPoints[n] = GLPoint(p0n[n].real(), p0n[n].imag(), 0);

                    wfr.drawBresenhamLine(ursprung, pnPoints[n], black_color);
                    int next = (n+1) % N;
                    wfr.drawBresenhamLine(pnPoints[n], p0nPoints[next], black_color);
                }
    **/
    /** Farben + Eckpunkte händisch + linien mit festen Punkten
             Color black_color(0,0,0);
             Color rot_color(1, 0, 0);
             Color orange_color(1, 0.5, 0);
             Color yellow_color(1, 1, 0);
             Color green_color(0,1,0);
             Color turquoise_color(0, 1, 1);
             Color blue_color(0, 0, 1);
             Color indigo_color(0.25, 0, 0.5);
             Color violet_color(0.5, 0, 0.75);

             GLPoint r(384,269,0);
             wfr.drawBresenhamLine(ursprung,r,black_color);
             GLPoint o(348,306,0);
             wfr.drawBresenhamLine(ursprung,o,black_color);
             GLPoint y(292,306,0);
             wfr.drawBresenhamLine(ursprung,y,black_color);
             GLPoint g(256,269,0);
             wfr.drawBresenhamLine(ursprung,g,black_color);
             GLPoint t(256,211,0);
             wfr.drawBresenhamLine(ursprung,t,black_color);
             GLPoint b(292,174,0);
             wfr.drawBresenhamLine(ursprung,b,black_color);
             GLPoint i(348,174,0);
             wfr.drawBresenhamLine(ursprung,i,black_color);
             GLPoint v(384,211,0);
             wfr.drawBresenhamLine(ursprung,v,black_color);

             //Generierung Windrad Punkte + Linien händisch
             GLPoint rw(377,265,0);wfr.drawBresenhamLine(rw,v,black_color);
             GLPoint ow(346,299,0);wfr.drawBresenhamLine(ow,r,black_color);
             GLPoint yw(296,299,0);wfr.drawBresenhamLine(yw,o,black_color);
             GLPoint gw(263,267,0);wfr.drawBresenhamLine(gw,y,black_color);
             GLPoint tw(263,215,0);wfr.drawBresenhamLine(tw,g,black_color);
             GLPoint bw(294,181,0);wfr.drawBresenhamLine(bw,t,black_color);
             GLPoint iw(344,181,0);wfr.drawBresenhamLine(iw,b,black_color);
             GLPoint vw(377,213,0);wfr.drawBresenhamLine(vw,i,black_color);
    **/

    /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier

        //Zufallsgenerator und Zufallsfarben
            std::default_random_engine engine(1);
            std::uniform_real_distribution<double> distribution(0.0, 1.0);
            Color farbe1_color(distribution(engine), distribution(engine), distribution(engine));
            Color farbe2_color(distribution(engine), distribution(engine), distribution(engine));

            Color borderColor = black_color; //Linienfarbe

            std::complex<double> o(cos(M_PI / 16.0), sin(M_PI / 16.0)); //Rotationsfaktor 11,25°
            for (int i = 0; i < eckpunkte_vector.size(); i++)
            {
                std::complex<double> rotiert = 0.5 * o * eckpunkte_vector[i];
                GLPoint startpunkt_gl_point(
                    (int)rotiert.real() + ursprung(0),
                    (int)rotiert.imag() + ursprung(1),
                    0.0); //Startpunkt für den seedfill

                //std::cout<<startpunkt_gl_point<<std::endl;
                Color fillColor = (i % 2 == 0) ? farbe1_color : farbe2_color;
                wfr.seedFillArea(startpunkt_gl_point, borderColor, fillColor); //seedfillaufruf
                //std::cout<<"Hat geklappt"<<std::endl;
            }
    */

    /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle*/
    //wfr.renderScene(black_color);

    //Bunny Blatt 2
    /*Model &bunny = scene->getModels()[0];
    bunny.setTranslation(GLVector(400, 200, 0));
    bunny.setRotation(GLVector(0, 5, 0));
    bunny.setScale(GLVector(2, 2, 2));*/

    //Cube Blatt 2
    /* cube_1.setTranslation(GLVector(150, 200, 0));
    cube_1.setRotation(GLVector(0, 45, -20));
    cube_1.setScale(GLVector(0.5, 3, 0.5));*/

    Model &cube = scene->getModels()[1];
    //cube kopien
    Model cube_2 = cube;
    Model cube_3 = cube;
    Model cube_4 = cube;

    scene->addModel(cube_2);
    scene->addModel(cube_3);
    scene->addModel(cube_4);

    /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */
    /*Color green_color(0, 1, 0);
    wfr.renderScene(green_color);*/ //nicht mehr benötigt von Blatt 2

    /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
    // Diese Einstellungen beziehen sich auf den world space
    // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
    // den ViewSpace benötigen, da die Strahen für Raycasting und Raytracing im
    // World space definiert sind. Modelle müssen also lediglich in den World
    // space überführt werden

    /* Aufgabenblatt 3:  kommentieren Sie die Zeilen wieder ein, die eine Kamera erzeugen und zur Scene hinzufügen*/

    auto cam = std::make_shared<Camera>();
    GLPoint eye = GLPoint(0.0, 0.0, 300.0);
    cam->setEyePoint(eye);
    cam->setUp(GLVector(0.0, 1.0, 0.0));
    GLVector viewDirection = GLVector(0.0, 0, -1.0);
    viewDirection.normalize();
    cam->setViewDirection(viewDirection);
    cam->setSize(img->getWidth(), img->getHeight());
    scene->setCamera(cam);

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
    sphere2.setPosition(GLPoint(150, 0.0, -30));

    /* Aufgabenblatt 3: (Wenn nötig) Transformationen der Modelle im World space, sodass sie von der Kamera gesehen werden könnnen. Die nötigen Transformationen für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */
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

    /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/
    Material cube1_material;
    cube1_material.color = Color(0.9, 0.9, 0.3);

    Material cube2_material;
    cube2_material.color = Color(0.9, 0.4, 0.3);

    Material cube3_material;
    cube3_material.color = Color(1.0, 0, 0); // 0.1 von Aufgabenblatt stimmt nicht mit Zielbild

    Material cube4_material;
    cube4_material.color = Color(0.9, 0.9, 0.9);

    Material materialsphere1 = Material();
    Color blue_color(0, 0, 1);
    materialsphere1.color = blue_color;
    materialsphere1.reflection = 1.0f; // Reflexion auf 1.0f gesetzt

    Material materialsphere2 = Material();
    Color tourquis_color(0, 1, 1);
    materialsphere2.color = tourquis_color;
    materialsphere2.reflection = 1.0f; // Reflexion auf 1.0f gesetzt

    Material materialbunny = Material();
    Color green_color(0, 1, 0);
    materialbunny.color = green_color;
    materialbunny.reflection = 1.0f; // Reflexion auf 1.0f gesetzt

    bunny.setMaterial(materialbunny);
    cube1.setMaterial(cube1_material);
    cube2.setMaterial(cube2_material);
    cube3.setMaterial(cube3_material);
    cube4.setMaterial(cube4_material);
    sphere1.setMaterial(materialsphere1);
    sphere2.setMaterial(materialsphere2);

    // Aufgabenblatt 1: Raum-Würfel hinzufügen (Cube 5)
    Model roomCube;
    roomCube.setScale(GLVector(500.0, 500.0, 500.0)); // Skalierung für den Raum-Würfel
    roomCube.setTranslation(GLVector(0.0, -100.0, 0.0)); // Translation für den Raum-Würfel
    Material roomMaterial;
    roomMaterial.color = Color(0.99, 0.99, 0.99); // Materialfarbe für den Raum-Würfel
    roomCube.setMaterial(roomMaterial);

    // Kopiere die Dreiecke des bereits geladenen Cube-Modells (Index 1) in den Raum-Würfel
    if (scene->getModels().size() > 1) {
        roomCube.mTriangles = scene->getModels()[1].mTriangles;
        // Normalen des Raum-Würfels umkehren, damit sie nach innen zeigen
        for (auto &triangle : roomCube.mTriangles) {
            triangle.normal = triangle.normal * -1.0;
        }
    } else {
        std::cerr << "Fehler: Cube-Modell (Index 1) nicht in der Szene geladen, kann den Raum-Würfel nicht erstellen." << std::endl;
    }
    scene->addModel(roomCube); // Füge den Raum-Würfel zur Szene hinzu.
    /* Aufgabenblatt 4: Setzen Sie materialeigenschaften für die Kugeln und die Modelle. Die Materialeigenschaften für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */


    /* Aufgabenblatt 4: Fügen Sie ein Licht zur Szene hinzu */



    scene->addPointLight(GLPoint(0.0, 200.0, 0.0));
    SolidRenderer solidRenderer(scene, img, cam);
    solidRenderer.renderRaycast();

    // Schreiben des Bildes in Datei

    if
    (argc
     >
     1
    ) {
        img->writeAsPPM(argv[1]);
        std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                << img->getHeight() << " in Datei " << argv[1] << " geschrieben."
                << std::endl;
    } else {
        std::cerr
                << "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
                << std::endl;
    }

    return
            0;
}


