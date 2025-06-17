// SolidRenderer.cpp Implementierung

#include "SolidRenderer.hpp"
#include <iostream>

//#include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/

void SolidRenderer::renderRaycast() {
    if (!mImage || !mScene || !mCamera) {
        std::cerr << "Fehler: mImage, mScene oder mCamera ist nicht gesetzt." << std::endl;
        return;
    }

    
    std::cout << "Rendern mittels Raycast gestartet." << std::endl;

    #pragma omp parallel for
    for(size_t i = 0; i < mImage->getHeight(); ++i ) {
        computeImageRow(i);
    }

    std::cout << "Rendern mittels Raycast abgeschlossen." << std::endl;
}


/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
    if(!mImage || !mScene || !mCamera) {
        std::cerr << "Fehler: mImage, mScene oder mCamera ist nicht gesetzt." << std::endl;
        return;
    }

    for (size_t x = 0; x < mImage->getWidth(); ++x) {
        Ray ray = mCamera->getRay(x, rowNumber);
        HitRecord hitRecord;
        hitRecord.recursions = 0; // Für Aufgabenblatt 4 wichtig, hier initial 0

        hitRecord.parameter = std::numeric_limits<double>::max();

        // Überprüfen, ob der Strahl ein Objekt in der Szene trifft
        if (mScene->intersect(ray, hitRecord, EPSILON)) {
            // Wenn ein Treffer vorliegt, schattiere das Pixel mit der Materialfarbe des Objekts
            // Für Aufgabenblatt 3 reicht es, die Farbe des getroffenen Objekts zu verwenden.
            // Die `shade` Funktion kann für Aufgabenblatt 4 erweitert werden.
            shade(hitRecord);
            mImage->setValue(x, rowNumber, hitRecord.color);
        } else {
            // Wenn kein Treffer vorliegt, setze das Pixel auf die Hintergrundfarbe (weiß)
            mImage->setValue(x, rowNumber, Color(1.0, 1.0, 1.0)); // Weiß als Hintergrundfarbe
        }
    }
}

/**
 * Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 * Für Aufgabenblatt 3 wird diese Funktion nur dazu verwendet, die Farbe des Materials zu setzen.
 */
void SolidRenderer::shade(HitRecord &r) {
    // Für Aufgabenblatt 3 reicht es, die Materialfarbe des getroffenen Objekts zu verwenden.
    // Die Farbinformation sollte bereits im HitRecord vorhanden sein, nachdem Scene::intersect aufgerufen wurde.
    // r.color ist bereits in Scene::intersect auf die Materialfarbe des getroffenen Objekts gesetzt worden.
    // Daher ist hier für Aufgabe 3 keine weitere Berechnung notwendig.
    if(r.modelId > -1) {
        r.color = mScene->getModels()[r.modelId].getMaterial().color;
    }
    if(r.sphereId > -1) {
        r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
    }
}