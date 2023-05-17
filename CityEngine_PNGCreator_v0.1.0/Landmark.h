#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>
#include "pngwriter.h"

class Landmark
{
protected:
	// bounds
	short minBoundX;
	short maxBoundX;
	short minBoundY;
	short maxBoundY;

	// center and range
	short centerX;
	short centerY;
	short rangeX;
	short rangeY;

	// probabilities
	short probLeft;
	short probDown;
	short probRight;
	short probUp;

	// sub area to make sure it doesn't interfere with a neighbor too much
	short minSubBoundX;
	short maxSubBoundX;
	short minSubBoundY;
	short maxSubBoundY;

	// color
	double red;
	double green;
	double blue;

	std::vector<short> filled;

	// private method used during generation
	bool FillNeighbor(short x, short y, std::vector<bool>* fillMap, std::vector<bool>* checkedMap, short prob);

public:
	// getters for bounds
	short GetMinBoundX();
	short GetMaxBoundX();
	short GetMinBoundY();
	short GetMaxBoundY();

	// constructor
	Landmark(short x1, short x2, short y1, short y2, double r, double g, double b);

	// draw methods
	void Draw();

	// takes in the main map, as well as the obstacle map and whether it should edit it
	// elevation is assumed to be false but it can be changed to true in the call
	// mainly because I was too lazy to make an elevation class, full transparency
	void Save(pngwriter* obs, pngwriter* main, bool obstacle, bool elevation = false);
};