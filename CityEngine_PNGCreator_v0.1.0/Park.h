#pragma once

/*
* Class: Park
* Parent: Landmark
* Author: Ryan Keller
* 
* Purpose:
* Represents a specific type of landmark - the park
* It takes up a quarter of the bounds (about the center)
* The obstacle is slightly larger (1/5th so)
* Is drawn as a rectangle (GL_POLYGON)
* Is saved with a square (square)
*/

#include "Landmark.h"
class Park :
    public Landmark
{
private:
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minObsX;
    double maxObsX;
    double minObsY;
    double maxObsY;

public:
    Park(double x1, double x2, double y1, double y2) :
        Landmark(x1, x2, y1, y2)
    {
        minX = 0.75 * x1 + 0.25 * x2;
        maxX = 0.75 * x2 + 0.25 * x1;
        minY = 0.75 * y1 + 0.25 * y2;
        maxY = 0.75 * y2 + 0.25 * y1;

        minObsX = 0.8 * x1 + 0.2 * x2;
        maxObsX = 0.8 * x2 + 0.2 * x1;
        minObsY = 0.8 * y1 + 0.2 * y2;
        maxObsY = 0.8 * y2 + 0.2 * y1;
    }

    void Draw();
    void Save(pngwriter* areas, pngwriter* obs, pngwriter* sky, pngwriter* top);
};