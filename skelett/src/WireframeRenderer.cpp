
#include "WireframeRenderer.hpp"
#include <stack>

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
    for (Model model : mScene -> getModels()) {
        GLMatrix transformation = model.getTransformation();
        for (Triangle triangle : model.mTriangles) {
            GLPoint p_1 = transformation*triangle.vertex[0];
            GLPoint p_2 = transformation*triangle.vertex[1];
            GLPoint p_3 = transformation*triangle.vertex[2];
            drawBresenhamLine(p_1, p_2, color);
            drawBresenhamLine(p_2, p_3, color);
            drawBresenhamLine(p_3, p_1, color);
        }
    }
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2)
**/

void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    int dx = static_cast<int>(p2(0)) - static_cast<int>(p1(0));
    int dy = static_cast<int>(p2(1)) - static_cast<int>(p1(1));

    int x = static_cast<int>(p1(0));
    int y = static_cast<int>(p1(1));

    //Oktant 1 & 2
    if(dx >= 0 && dy >= 0){
        //Oktant 1
        if (dx >= dy) {
            int e = 2 * dy - dx;

            for (int i = 1; i <= dx; i++) {
                // Schleife für x1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und y erhoehen
                    y++;
                    e = e - 2 * dx;
                }
                x++;
                e = e + 2 * dy;
            }
            mImage->setValue(x, y, color);
        }
        //Oktant 2
        else if(dx < dy){
            int e = 2 * dx - dy;

            for (int i = 1; i <= dy; i++) {
                // Schleife für y1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und x erhoehen
                    x++;
                    e = e - 2 * dy;
                }
                y++;
                e = e + 2 * dx;
            };
            mImage->setValue(x, y, color);
        };
    }

    //Oktant 3 & 4
    else if(dx < 0 && dy > 0){
        dx = -dx;
        //x = -x;
        //Oktant 3
        if(dx < dy){
            int e = 2 * dx - dy;

            for (int i = 1; i <= dy; i++) {
                // Schleife für y1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und x erhoehen
                    //x++;
                    x--;
                    e = e - 2 * dy;
                }
                y++;
                e = e + 2 * dx;
            };
            mImage->setValue(x, y, color);
        }

        //Oktant 4
        else if(dx >= dy) {
            int e = 2 * dy - dx;

            for (int i = 1; i <= dx; i++) {
                // Schleife für x1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und y erhoehen
                    y++;
                    e = e - 2 * dx;
                }
                //x++;
                x--;
                e = e + 2 * dy;
            }
            mImage->setValue(x, y, color);
        }

    }

    //Oktant 5 & 6
    else if(dx <= 0 && dy <= 0){
        dx = -dx;
        dy = -dy;
        //x = -x;
        //y = -y;

        //Oktant 5
        if(dx >= dy){
            int e = 2 * dy - dx;

            for (int i = 1; i <= dx; i++) {
                // Schleife für x1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und y erhoehen
                    //y++;
                    y--;
                    e = e - 2 * dx;
                }
                //x++;
                x--;
                e = e + 2 * dy;
            }
            mImage->setValue(x, y, color);
        }

        //Oktant 6
        else if(dx < dy){
            int e = 2 * dx - dy;

            for (int i = 1; i <= dy; i++) {
                // Schleife für y1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und x erhoehen
                    //x++;
                    x--;
                    e = e - 2 * dy;
                }
                //y++;
                y--;
                e = e + 2 * dx;
            };
            mImage->setValue(x, y, color);
        }
    }

    //Oktant 7 & 8
    else if (dx > 0 && dy < 0){
        dy = -dy;
        //y = -y;

        //Oktant 7
        if(dx < dy){
            int e = 2 * dx - dy;

            for (int i = 1; i <= dy; i++) {
                // Schleife für y1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und x erhoehen
                    x++;
                    e = e - 2 * dy;
                }
                //y++;
                y--;
                e = e + 2 * dx;
            };
            mImage->setValue(x, y, color);
        }

        //Oktant 8
        else if(dx >= dy){
            int e = 2 * dy - dx;

            for (int i = 1; i <= dx; i++) {
                // Schleife für x1
                mImage->setValue(x, y, color);
                if (e >= 0) {
                    //oberen Punkt zeichnen und y erhoehen
                    //y++;
                    y--;
                    e = e - 2 * dx;
                }
                x++;
                e = e + 2 * dy;
            }
            mImage->setValue(x, y, color);
        }
    }
}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor)
{
    //Julias Version
    if (mImage->getValues()[seed(1)* mImage->getWidth() +seed(0)] != borderColor && mImage->getValues()[seed(1)* mImage->getWidth() +seed(0)] != fillColor)
    {

        std::stack<GLPoint> pixel_stack;
        pixel_stack.push(seed);


        while(!pixel_stack.empty())
        {
            GLPoint pixel = pixel_stack.top();
            pixel_stack.pop();
            mImage->setValue(pixel(0),pixel(1), fillColor);

            std::vector<GLPoint> connected_pixel;
            GLPoint rechts(pixel(0)-1,pixel(1),pixel(2));
            GLPoint oben(pixel(0),pixel(1)+1,pixel(2));
            GLPoint links(pixel(0)+1,pixel(1),pixel(2));
            GLPoint unten(pixel(0),pixel(1)-1,pixel(2));
            connected_pixel.push_back(rechts);
            connected_pixel.push_back(oben);
            connected_pixel.push_back(links);
            connected_pixel.push_back(unten);
            for (GLPoint point : connected_pixel)
            {
                if (mImage->getValues()[point(1)* mImage->getWidth() +point(0)] != borderColor && mImage->getValues()[point(1)* mImage->getWidth() +point(0)] != fillColor)
                {
                    pixel_stack.push(point);
                }
            }
        }
    }

    /*Helenes Version
    //std::tuple<int, int> pixel = std::make_tuple(static_cast<int>(seed(0)), static_cast<int>(seed(1)));
    //std::stack<int, int> stack;
    std::tuple<int, int> pixel;
    std::stack<std::tuple<int,int>> stack;

    stack.push(std::make_tuple(static_cast<int>(seed(0)), static_cast<int>(seed(1))));
    while(!(stack.empty())){
    //for (int i; i<4000; i++){
        pixel = stack.top();
        stack.pop();

        int x = std::get<0>(pixel);
        int y = std::get<1>(pixel);
        mImage->setValue(x, y, fillColor);
        //std::cout << x << ' ' << y << '\n';

        //std::cout << x << ' ' << y << '\n';


        for(int i = -1; i <=1; i+=2){
            Color actualColor = mImage->getValue(x+i,y);
            if( (actualColor != borderColor) && (actualColor != fillColor) && (mImage->getWidth() > x+i) && (x+i >=0 ) ){
                stack.push(std::make_tuple(x+i, y));
                //std::cout << x+i << ' ' << y << '\n';
            }
        }
        for(int j=-1; j <= 1; j+=2){
            Color actualColor = mImage->getValue(x,y+j);
            if( (actualColor != borderColor) && (actualColor != fillColor) && (mImage->getHeight() > y+j) && (y+j >=0) ){
                stack.push(std::make_tuple(x, y+j));
                //std::cout << x << ' ' << y+j << '\n';
            }
        }
    }
    */
}
