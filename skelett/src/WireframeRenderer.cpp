
#include "WireframeRenderer.hpp"
#include "math.hpp"


Image *mImage;
Scene *mScene;

void WireframeRenderer::renderScene(Color color) {
    // Durchlaufe alle Modelle
    for (const auto& model : mScene->getModels()) {
        // Jedes Modell besteht aus einer Liste von Dreiecken
        for (const auto& triangle : model.mTriangles) {
            // Transformiere die Eckpunkte des Dreiecks
            GLPoint v0_transformed = model.mTransform * triangle.vertex[0];
            GLPoint v1_transformed = model.mTransform * triangle.vertex[1];
            GLPoint v2_transformed = model.mTransform * triangle.vertex[2];

            // Verbinde die transformierten Punkte mit Linien
            drawBresenhamLine(v0_transformed, v1_transformed, color);
            drawBresenhamLine(v1_transformed, v2_transformed, color);
            drawBresenhamLine(v2_transformed, v2_transformed, color);
        }
    }
}

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/


/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2)
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color)
{
    if (mImage != nullptr){
        int dx = int(p2.operator()(0) - p1.operator()(0));
        int dy = int(p2.operator()(1) - p1.operator()(1));
        
        if(dx >= 0){
            //1, 2, 7, 8 oktant
            if (dy >= 0){
                //1, 2 oktant
                if (abs(dx) >= abs(dy)) drawBresenham1(p1, p2, color);     //1. oktant
                else drawBresenham2(p1, p2, color);              //2. oktant
            }
            else {
                //7, 8 oktant
                if (abs(dx) >= abs(dy)) drawBresenham8(p1, p2, color);     //8. oktant
                else drawBresenham7(p1, p2, color);              //7. oktant
            }
        }
        else {
            //3, 4, 5, 6 oktant
            if (dy >= 0){
                //3, 4 oktant
                if (abs(dx) >= abs(dy)) drawBresenham4(p1, p2, color);     //4. oktant
                else drawBresenham3(p1, p2, color);              //3. oktant
            }
            else {
                //5, 6 oktant
                if (abs(dx) >= abs(dy)) drawBresenham5(p1, p2, color);     //5. oktant
                else drawBresenham6(p1, p2, color);              //6. oktant
            }
        }
    }
}


void WireframeRenderer::drawBresenham1(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = dy + dy - dx;
    
    for (;x <= p2.operator()(0); x++){
        mImage->setValue(x, y, color);
        if (e > 0){
            y++; 
            e -= dx + dx;
        }
        e += dy + dy;
    }
}

void WireframeRenderer::drawBresenham2(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = dx + dx - dy;
    
    for (;y <= p2.operator()(1); y++){
        mImage->setValue(x, y, color);
        if (e > 0){
            x++; 
            e -= dy + dy;
        }
        e += dx + dx;
    }
}

void WireframeRenderer::drawBresenham3(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = abs(dx) + abs(dx) - abs(dy);
    
    for (;y <= p2.operator()(1); y++){
        mImage->setValue(x, y, color);
        if (e > 0){
            x--; 
            e -= dy + dy;
        }
        e += abs(dx) + abs(dx);
    }
}

void WireframeRenderer::drawBresenham4(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = abs(dy) + abs(dy) - abs(dx);
    
    for (;x >= p2.operator()(0); x--){
        mImage->setValue(x, y, color);
        if (e > 0){
            y++; 
            e -= abs(dx) + abs(dx);
        }
        e += abs(dy) + abs(dy);
    }
}

void WireframeRenderer::drawBresenham5(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = abs(dy) + abs(dy) - abs(dx);
    
    for (; x >= p2.operator()(0); x--){
        mImage->setValue(x, y, color);
        if (e > 0){
            y--; 
            e -= abs(dx) + abs(dx);
        }
        e += abs(dy) + abs(dy);
    }
}

void WireframeRenderer::drawBresenham6(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = abs(dx) + abs(dx) - abs(dy);
    
    for (; y >= p2.operator()(1); y--){
        mImage->setValue(x, y, color);
        if (e > 0){
            x--; 
            e -= abs(dy) + abs(dy);
        }
        e += abs(dx) + abs(dx);
    }
}

void WireframeRenderer::drawBresenham7(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = int(p2.operator()(0)) - x;
    dy = int(p2.operator()(1)) - y;
    e = abs(dx) + abs(dx) - abs(dy);
    
    for (; y >= p2.operator()(1); y--){
        mImage->setValue(x, y, color);
        if (e > 0){
            x++; 
            e -= abs(dy) + abs(dy);
        }
        e += abs(dx) + abs(dx);
    }
}

void WireframeRenderer::drawBresenham8(GLPoint p1, GLPoint p2, Color color){
    int x, y, dx, dy, e;
    x = int(p1.operator()(0));
    y = int(p1.operator()(1));
    dx = abs(int(p2.operator()(0)) - x);
    dy = abs(int(p2.operator()(1)) - y);
    e = dy + dy - dx;
    
    for (;x <= p2.operator()(0); x++){
        mImage->setValue(x, y, color);
        if (e > 0){
            y--; 
            e -= dx + dx;
        }
        e += dy + dy;
    }
}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
    int x = (int)seed.operator()(0); 
    int y = (int)seed.operator()(1);
    std::vector<int> trailx = {};
    std::vector<int> traily = {};
    while(true){
        if(mImage->getValues()[(y - 1) * mImage->getWidth() + x] != borderColor &&
           mImage->getValues()[(y - 1) * mImage->getWidth() + x] != fillColor ){
            y--;
            mImage->setValue(x, y, fillColor);
            trailx.push_back(x);
            traily.push_back(y);
        }
        else if(mImage->getValues()[y * mImage->getWidth() + (x + 1)] != borderColor &&
                mImage->getValues()[y * mImage->getWidth() + (x + 1)] != fillColor ){
            x++;
            mImage->setValue(x, y, fillColor);
            trailx.push_back(x);
            traily.push_back(y);
        }
        else if(mImage->getValues()[(y + 1) * mImage->getWidth() + x] != borderColor &&
                mImage->getValues()[(y + 1) * mImage->getWidth() + x] != fillColor ){
            y++;
            mImage->setValue(x, y, fillColor);
            trailx.push_back(x);
            traily.push_back(y);
        }
        else if(mImage->getValues()[y * mImage->getWidth() + (x - 1)] != borderColor &&
                mImage->getValues()[y * mImage->getWidth() + (x - 1)] != fillColor ){
            x--;
            mImage->setValue(x, y, fillColor);
            trailx.push_back(x);
            traily.push_back(y);
        }
        else if (trailx.empty()){
            break;
        }
        else {
            trailx.pop_back();
            traily.pop_back();
            if (!trailx.empty()){
                x = trailx.back();
                y = traily.back();
            } 
        }
    }
}

