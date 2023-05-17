#include "Landmark.h"

// getters for the boundaries
short Landmark::GetMinBoundX()
{
    return minBoundX;
}

short Landmark::GetMaxBoundX()
{
    return maxBoundX;
}

short Landmark::GetMinBoundY()
{
    return minBoundY;
}

short Landmark::GetMaxBoundY()
{
    return maxBoundY;
}

/// <summary>
/// constructor - boundaries and color
/// </summary>
/// <param name="x1"></param>
/// <param name="x2"></param>
/// <param name="y1"></param>
/// <param name="y2"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
Landmark::Landmark(short x1, short x2, short y1, short y2, double r, double g, double b)
{
    // store bounds
    minBoundX = x1;
    maxBoundX = x2;
    minBoundY = y1;
    maxBoundY = y2;

    // calculate center and range
    centerX = (x1 + x2) / 2;
    centerY = (y1 + y2) / 2;
    rangeX = x2 - x1;
    rangeY = y2 - y1;

    // used for keeping track of rolls
    short maxRoll = 8;
    short baseRoll = 1;
    short highRolls = 0;
    short lowRolls = 0;
    short mediumRolls = 0;

    do
    {
        // reset
        maxRoll = 8;
        baseRoll = 1;
        highRolls = 0;
        lowRolls = 0;
        mediumRolls = 0;

        // set left probability and log result
        probLeft = rand() % maxRoll + baseRoll;
        if (probLeft > 6)
        {
            highRolls++;
        }
        else if (probLeft < 3)
        {
            lowRolls++;
        }
        else
        {
            mediumRolls++;
        }

        // set down probability and log result
        probDown = rand() % maxRoll + baseRoll;
        if (probDown > 6)
        {
            highRolls++;
        }
        else if (probDown < 3)
        {
            lowRolls++;
        }
        else
        {
            mediumRolls++;
        }

        // set right probability and log result
        probRight = rand() % maxRoll + baseRoll;
        if (probRight > 6)
        {
            highRolls++;
        }
        else if (probRight < 3)
        {
            lowRolls++;
        }
        else
        {
            mediumRolls++;
        }

        // set up probability and log result
        probUp = rand() % maxRoll + baseRoll;
        if (probUp > 6)
        {
            highRolls++;
        }
        else if (probUp < 3)
        {
            lowRolls++;
        }
        else
        {
            mediumRolls++;
        }

        // repeat until there is an even spread of rolls (prevents squares)
    } while (mediumRolls < 1 || highRolls < 1 || lowRolls < 1);

    // calculate inner bounds
    minSubBoundX = (9*x1 + x2) / 10;
    maxSubBoundX = (x1 + 9*x2) / 10;
    minSubBoundY = (9*y1 + y2) / 10;
    maxSubBoundY = (y1 + 9*y2) / 10;

    // store colors
    red = r;
    green = g;
    blue = b;

    // initialize filled
    filled = std::vector<short>();

    // fill center
    filled.push_back(centerX);
    filled.push_back(centerY);

    // used for "hit detection"
    bool hit = false;

    // fill map used to store whether it's filled or not
    std::vector<bool>* fillMap = new std::vector<bool>();
    fillMap->resize(rangeX * rangeY);

    // checked map used to store whether it's been checked in this while iteration
    std::vector<bool>* checkedMap = new std::vector<bool>();
    checkedMap->resize(rangeX * rangeY);

    // until a fill request hits an inner boundary
    while (!hit)
    {
        // run through the filled list
        for (int i = 0; i < filled.size(); i += 2)
        {
            // attempt to fill each neighbor, and check if anything hit an inner boundary
            hit = FillNeighbor(filled[i] - 1, filled[i + 1], fillMap, checkedMap, probLeft) ||
                FillNeighbor(filled[i], filled[i + 1] - 1, fillMap, checkedMap, probDown) ||
                FillNeighbor(filled[i] + 1, filled[i + 1], fillMap, checkedMap, probRight) ||
                FillNeighbor(filled[i], filled[i + 1] + 1, fillMap, checkedMap, probUp);
        }

        // clear the checked map
        checkedMap->clear();
        checkedMap->resize(rangeX * rangeY);
    }
}

/// <summary>
/// used to try to fill a neighbor
/// </summary>
/// <param name="x">neighbor's x</param>
/// <param name="y">neighbor's y</param>
/// <param name="fillMap">map to check if already filled</param>
/// <param name="checkedMap">map to check if already checked this loop</param>
/// <param name="prob">probability of filling</param>
/// <returns>true upon encountering the inner boundary of the class</returns>
bool Landmark::FillNeighbor(short x, short y, std::vector<bool>* fillMap, std::vector<bool>* checkedMap, short prob)
{
    // if filled, skip
    if ((*fillMap)[(x - minBoundX) * rangeY + (y - minBoundY)]) return false;
    if ((*checkedMap)[(x - minBoundX) * rangeY + (y - minBoundY)]) return false;

    // if on the edge of the sub area, end algo
    if (x == minSubBoundX ||
        x == maxSubBoundX ||
        y == minSubBoundY ||
        y == maxSubBoundY)
    {
        return true;
    }

    // otherwise, try to fill it (70% chance)
    if (rand() % 10 < prob)
    {
        (*fillMap)[(x - minBoundX) * rangeY + (y - minBoundY)] = true;
        filled.push_back(x);
        filled.push_back(y);
    }

    (*checkedMap)[(x - minBoundX) * rangeY + (y - minBoundY)] = true;
    return false;
}

/// <summary>
/// draw a pseudo pixel at each location in the filled list using OpenGL
/// </summary>
void Landmark::Draw()
{
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);

    // cycle through filled list
    for (int i = 0; i < filled.size(); i += 2)
    {
        // draw four points to make a pseuso pixel
        glVertex2f(filled[i], filled[i+1]);
        glVertex2f(filled[i] + 1, filled[i + 1]);
        glVertex2f(filled[i] + 1, filled[i + 1] + 1);
        glVertex2f(filled[i], filled[i + 1] + 1);
    }

    glEnd();
}

/// <summary>
/// save the landmark to the give png writers
/// </summary>
/// <param name="obs">obstacle map</param>
/// <param name="main">preferred map</param>
/// <param name="obstacle">is an obstacle?</param>
/// <param name="elevation">is an elevation?</param>
void Landmark::Save(pngwriter* obs, pngwriter* main, bool obstacle, bool elevation)
{
    // if elevation
    if (elevation)
    {
        // do a very broad blend
        for (int i = 0; i < filled.size(); i += 2)
        {
            main->filledsquare_blend(filled[i] - 100, filled[i + 1] - 100, filled[i] + 100, filled[i + 1] + 100, 0.0003, red, green, blue);
        }

        return;
    }

    // if obstacle
    if (obstacle)
    {
        // save a crisp recreation to the main map in color and to the obstacle map in black
        for (int i = 0; i < filled.size(); i += 2)
        {
            obs->square(filled[i], filled[i + 1], filled[i] + 1, filled[i + 1] + 1, 0, 0, 0);
            main->square(filled[i], filled[i + 1], filled[i] + 1, filled[i + 1] + 1, red, green, blue);
        }
    }

    // otherwise
    else
    {
        // draw a medium blend (mostly used for skyline)
        for (int i = 0; i < filled.size(); i += 2)
        {
            main->filledsquare_blend(filled[i] - 10, filled[i + 1] - 10, filled[i] + 10, filled[i + 1] + 10, 0.01, red, green, blue);
        }
    }
}