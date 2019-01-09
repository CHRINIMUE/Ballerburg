#pragma once

#include "defines.h"

#include "opencv2/opencv.hpp"
using namespace cv;

class Mountain {
public:

	Mountain();

	int calcDistance(int ax, int ay, int bx, int by, int createdMountainPoints, int amountPoints, int xy);

	void draw(Mat& frame);

private:
	Point MountainPoints[1][500];
	int CreatedMountainPoints  = 0;
	int MountainHeight = rand() % ((GROUND_HEIGHT - 100) - (WINHEIGHT / 8)) + (WINHEIGHT / 8);
	int MountainLeft = rand() % (WINWIDTH / 3 - 200) + 200;
	int MountainRight = rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 200)) + ((WINWIDTH / 3 * 2) - 1);;
	int MountainWidth = MountainRight - MountainLeft;			// Calc the width (X) of the rock
	int MountainMiddle = (MountainWidth / 2) + MountainLeft;	// Calc the middle x cordinate of the rock
	int LeftPoints = 0;								// Amount of Points set left from the middle
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points
};