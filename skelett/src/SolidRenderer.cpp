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

        // Cora 4.4
        hit.recursions = 0; // Der erste Strahl hat Rekursionstiefe 0
        hit.rayDirection = ray.direction; // Die Richtung des aktuellen Strahls speichern
        // Cora 4.4

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
//Cora 4.2&3


void SolidRenderer::shade(HitRecord &r) {
    // Ein sehr kleiner Wert, um Selbst-Schnitt zu vermeiden
    const float SHADOW_EPSILON = 1e-4;

    // Material des getroffenen Objekts holen
    Material material;
    if (r.modelId != -1) {
        material = mScene->getModels()[r.modelId].getMaterial();
    } else if (r.sphereId != -1) {
        material = mScene->getSpheres()[r.sphereId].getMaterial();
    } else {
        r.color = Color(0,0,0);
        return;
    }

    // Vektoren für die Beleuchtung vorbereiten
    GLPoint intersectionPoint = r.intersectionPoint;
    GLVector normal = r.normal;
    GLPoint eyePoint = mCamera->getEyePoint();
    GLVector viewVec = eyePoint - intersectionPoint;
    viewVec.normalize();

    // Vorder- und Rückseiten gleich beleuchten
    if (dotProduct(normal, viewVec) < 0) {
        normal = normal * -1.0;
    }


    // Cora 4.4

    // Fall 1: Das Material ist reflektierend
    if (material.reflection > 0.0) {
        // 1. Berechne den Reflexionsstrahl
        GLVector reflectDir = r.rayDirection - 2.0 * dotProduct(r.rayDirection, normal) * normal;
        reflectDir.normalize();

        Ray reflectionRay;
        reflectionRay.direction = reflectDir;
        // 2. Ursprung leicht verschieben, um Selbst-Schnitt zu vermeiden [cite: 193]
        reflectionRay.origin = intersectionPoint + normal * SHADOW_EPSILON;

        // 3. Neuen HitRecord für den rekursiven Aufruf vorbereiten [cite: 195]
        HitRecord reflectionHit;
        reflectionHit.parameter = std::numeric_limits<float>::infinity();
        reflectionHit.recursions = r.recursions + 1; // Rekursionstiefe erhöhen [cite: 196]
        reflectionHit.rayDirection = reflectionRay.direction;

        // 4. Schnitttest für den Reflexionsstrahl durchführen
        if (mScene->intersect(reflectionRay, reflectionHit, SHADOW_EPSILON)) {
            // Wenn etwas getroffen wurde, rufe shade() rekursiv auf [cite: 198]
            shade(reflectionHit);
            r.color = reflectionHit.color; // Das Ergebnis der Spiegelung ist unsere Farbe
        } else {
            // Wenn nichts getroffen wird, spiegelt das Objekt den "Hintergrund"
            // In unserem Fall die fast weiße Farbe des Raum-Würfels oder Weiß
            r.color = Color(1.0, 1.0, 1.0);
        }
        return; // Für perfekt spiegelnde Objekte sind wir hier fertig.
    }

    // Cora 4.4

    // Fall 2: Das Material ist NICHT reflektierend (Code aus Aufgabe 2 & 3)
    // Ambiente Komponente ist immer vorhanden, auch im Schatten
    Color ambientPart = material.ambient;
    ambientPart.r *= material.color.r;
    ambientPart.g *= material.color.g;
    ambientPart.b *= material.color.b;

    Color finalColor = ambientPart;

    // Diffuse und spekulare Komponenten für jede Lichtquelle berechnen
    for (const auto& lightPos : mScene->getPointLights()) {
        GLVector lightVecUnnormalized = lightPos - intersectionPoint;
        double distanceToLight = lightVecUnnormalized.norm();
        GLVector lightVec = lightVecUnnormalized;
        lightVec.normalize();

        // --- ANFANG: Schattenberechnung für Aufgabe 3 ---

        // 1. Erstelle einen neuen Schattenstrahl
        Ray shadowRay;
        // 2. Verschiebe den Ursprung, um Selbst-Schnitt zu vermeiden
        shadowRay.origin = intersectionPoint + (normal * SHADOW_EPSILON);
        shadowRay.direction = lightVec;

        // 3. Erstelle einen HitRecord für den Schattentest
        HitRecord shadowHit;
        // Wichtig: Wir suchen nur nach Objekten ZWISCHEN Punkt und Licht
        shadowHit.parameter = distanceToLight;

        // 4. Führe den Schnitttest für den Schattenstrahl durch
        bool inShadow = mScene->intersect(shadowRay, shadowHit, SHADOW_EPSILON);

        // --- ENDE: Schattenberechnung für Aufgabe 3 ---

        // 5. Nur wenn NICHT im Schatten, füge diffuse und spekulare Anteile hinzu
        if (!inShadow) {
            // Diffuser Anteil
            float NdotL = std::max(0.0f, (float)dotProduct(normal, lightVec));
            Color diffusePart = material.diffuse * NdotL;
            diffusePart.r *= material.color.r;
            diffusePart.g *= material.color.g;
            diffusePart.b *= material.color.b;

            // Spekularer Anteil
            GLVector reflectVec = 2.0 * dotProduct(lightVec, normal) * normal - lightVec;
            reflectVec.normalize();
            float RdotV = std::max(0.0f, (float)dotProduct(reflectVec, viewVec));
            Color specularPart = material.specular * pow(RdotV, material.shininess);

            // Farbe aufaddieren
            finalColor += diffusePart;
            finalColor += specularPart;
        }
    }

    // Sicherstellen, dass die Farbwerte nicht über 1.0 gehen
    finalColor.r = std::min(1.0f, finalColor.r);
    finalColor.g = std::min(1.0f, finalColor.g);
    finalColor.b = std::min(1.0f, finalColor.b);

    r.color = finalColor;
}