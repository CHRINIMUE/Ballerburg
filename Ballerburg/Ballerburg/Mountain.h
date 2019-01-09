#pragma once

#include "defines.h"

#include "opencv2/opencv.hpp"
using namespace cv;

class Mountain {
public:

	Mountain();

	int CalcDistance(int ax, int ay, int bx, int by, int createdMountainPoints, int amountPoints, int xy);

	void draw(Mat& frame);

private:
	Point mountainPoints[1][500];
	int createdMountainPoints  = 0;
	int groundHeight = GROUND_HEIGHT;	// generate the bottom ground
	int rockHeight = rand() % ((groundHeight - 100) - (WINHEIGHT / 4)) + (WINHEIGHT / 4);			// generate the height of the rock
	int rockLeft = rand() % (WINWIDTH / 3 - 200) + 200;												// generate start of the rock in 1/3 and 150px away from the border
	int rockRight = rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 200)) + ((WINWIDTH / 3 * 2) - 1);	// generate end of the rock in 3/3 and 150px away from the border
	int rockWidth = rockRight - rockLeft;			// Calc the width (X) of the rock
	int rockMiddle = (rockWidth / 2) + rockLeft;	// Calc the middle x cordinate of the rock
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points
	int leftPoints = 0;								// Amount of Points set left from the middle

};