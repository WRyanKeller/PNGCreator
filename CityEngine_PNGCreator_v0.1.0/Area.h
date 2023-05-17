#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>
#include "pngwriter.h"

class Area
{
protected:
	// bounds
	short minBoundX;
	short maxBoundX;
	short minBoundY;
	short maxBoundY;

	short centerX;
	short centerY;
	short rangeX;
	short rangeY;

	short probLeft;
	short probDown;
	short probRight;
	short probUp;

	short minSubBoundX;
	short maxSubBoundX;
	short minSubBoundY;
	short maxSubBoundY;

	double red;
	double green;
	double blue;

	std::vector<short> filled;

	bool FillNeighbor(short x, short y, std::vector<bool>* fillMap, std::vector<bool>* checkedMap, short prob);

public:
	// getters for bounds
	short GetMinBoundX();
	short GetMaxBoundX();
	short GetMinBoundY();
	short GetMaxBoundY();

	Area(short x1, short x2, short y1, short y2, double r, double g, double b);

	void Draw();
	void Save(pngwriter* areas);
};