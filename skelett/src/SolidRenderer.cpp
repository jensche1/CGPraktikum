// SolidRenderer.cpp Implementierung

#include "SolidRenderer.hpp"
#include <iostream>
// <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
  std::cout << "Rendern mittels Raycast gestartet." << std::endl;



  #pragma omp parallel for
  for(size_t i = 0; i < mImage->getHeight(); ++i )
  {
         computeImageRow( i );
      }

  std::cout << "Rendern mittels Raycast abgeschlossen." << std::endl;
}

/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
    for (size_t spalte = 0; spalte < mImage->getWidth(); ++spalte) {   //Fehlersuche: Noch einmal neu, falls hier möglicher Fehler.
        Ray ray = mCamera->getRay(spalte, rowNumber);
        HitRecord hit;
        hit.color = Color (0,0,0);
		hit.parameter = std::numeric_limits<float>::infinity();
        hit.modelId = -1;
        hit.sphereId = -1;
        hit.triangleId = -1;
        // Überprüfen, ob der Strahl ein Objekt in der Szene trifft
        if (mScene->intersect(ray, hit, EPSILON)) {
            shade(hit);
            mImage->setValue(spalte, rowNumber, hit.color);
        } else {
            // Wenn kein Treffer vorliegt, setze das Pixel auf die Hintergrundfarbe (weiß)
            mImage->setValue(spalte, rowNumber, Color(1, 1, 1));
        }
    }
}

/**
 * Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 * Für Aufgabenblatt 3 wird diese Funktion nur dazu verwendet, die Farbe des Materials zu setzen.
 */
void SolidRenderer::shade(HitRecord &r) {
    if (r.modelId !=-1) {
        r.color = mScene->getModels()[r.modelId].getMaterial().color;
    }
    if (r.sphereId !=-1) {
        r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
    }
}