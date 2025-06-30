// SolidRenderer.cpp
#include "SolidRenderer.hpp"
#include <algorithm> // For std::max
#include <cmath>     // For std::pow

#define EPSILON (1e-12) // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
    std::cout << "Rendern mittels Raycast gestartet." << std::endl;

    #pragma omp parallel for
    for (size_t i = 0; i < mImage->getHeight(); ++i) {
        computeImageRow(i);
    }
}

/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
    for (size_t spalte = 0; spalte < mImage->getWidth(); ++spalte) {
        Ray ray = mCamera->getRay(spalte, rowNumber); // Strahl für Pixel

        HitRecord hitRecord;
        // Initialize hitRecord.parameter to infinity to find the closest hit
        hitRecord.parameter = std::numeric_limits<double>::infinity();
        hitRecord.modelId = -1;
        hitRecord.sphereId = -1;

        if (mScene->intersect(ray, hitRecord, EPSILON)) {
            // Object hit, now shade the point
            shade(hitRecord);
            mImage->setValue(spalte, rowNumber, hitRecord.color);
        } else {
            // No object hit, set background color (e.g., black)
            mImage->setValue(spalte, rowNumber, Color(0.0, 0.0, 0.0));
        }
    }
}

/**
 * Aufgabenblatt 3: Hier wird das Raytracing/Shading implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
    // --- Beleuchtungsparameter (gemäß Aufgabenblatt) ---
    const float k_ambient = 0.4f;
    const float k_diffuse = 0.4f;
    const float k_specular = 0.2f;
    const float n = 20.0f; // Glanzlicht-Exponent
    const Color light_intensity = Color(1.0, 1.0, 1.0); // Weißes Licht

    // Materialfarbe
    Color material_color;
    float reflection = 0.0f;

    if (r.modelId != -1) {
        const auto &mat = mScene->getModels()[r.modelId].getMaterial();
        material_color = mat.color;
        reflection = mat.reflection;
    } else if (r.sphereId != -1) {
        const auto &mat = mScene->getSpheres()[r.sphereId].getMaterial();
        material_color = mat.color;
        reflection = mat.reflection;
    } else {
        material_color = r.color; // Fallback
    }

    // Startfarbe: Ambient-Anteil
    Color final_color = k_ambient * material_color * light_intensity;

    // Jede Lichtquelle verarbeiten
    for (const auto &light_position : mScene->getPointLights()) {
        GLVector L_unnormalized = light_position - r.intersectionPoint;
        double light_distance = L_unnormalized.norm();
        GLVector L = L_unnormalized;
        L.normalize();

        GLVector N = r.normal;
        N.normalize();

        // Schattenstrahl vorbereiten
        Ray shadow_ray;
        shadow_ray.origin = r.intersectionPoint + N * EPSILON;
        shadow_ray.direction = L;

        HitRecord shadow_hit;
        shadow_hit.parameter = light_distance;
        shadow_hit.modelId = -1;
        shadow_hit.sphereId = -1;

        bool is_shadowed = false;
        if (mScene->intersect(shadow_ray, shadow_hit, EPSILON)) {
            if (shadow_hit.parameter < light_distance) {
                is_shadowed = true;
            }
        }

        if (!is_shadowed) {
            // Diffusanteil (Lambert)
            double diffuse_factor = std::max(0.0, dotProduct(L, N));
            final_color += k_diffuse * material_color * light_intensity * diffuse_factor;

            // Spekularanteil (Phong)
            GLVector V = mCamera->getEyePoint() - r.intersectionPoint;
            V.normalize();
            GLVector R = 2.0 * dotProduct(L, N) * N - L;
            R.normalize();

            double specular_factor = std::pow(std::max(0.0, dotProduct(R, V)), n);
            final_color += k_specular * light_intensity * specular_factor;
        }
        // Optional: bei Schatten könnte man final_color *= 0.5; laut Aufgabenblatt
    }

    // --- Reflexion (Aufgabe 4) ---
    if (reflection > 0.0f) {
        const int maxRecursions = 2;
        if (r.recursions < maxRecursions) {
            GLVector N = r.normal;
            N.normalize();
            GLVector D = r.rayDirection;
            D.normalize();

            GLVector R = D - 2.0 * dotProduct(D, N) * N;
            R.normalize();

            Ray reflection_ray;
            reflection_ray.origin = r.intersectionPoint + N * EPSILON;
            reflection_ray.direction = R;

            HitRecord reflected_hit;
            reflected_hit.parameter = std::numeric_limits<double>::infinity();
            reflected_hit.modelId = -1;
            reflected_hit.sphereId = -1;
            reflected_hit.recursions = r.recursions + 1;

            if (mScene->intersect(reflection_ray, reflected_hit, EPSILON)) {
                shade(reflected_hit);
                r.color = reflected_hit.color;
                return;
            } else {
                r.color = Color(0.0, 0.0, 0.0); // Hintergrund
                return;
            }
        }
    }

    // Wenn keine Reflexion oder maximale Tiefe erreicht
    r.color = final_color;
}
