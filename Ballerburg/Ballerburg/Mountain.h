#pragma once

#include "defines.h"

#include "opencv2/opencv.hpp"
using namespace cv;

class Mountain {
public:

	Mountain(int MinHeight);

	int calcDistance(int ax, int ay, int bx, int by, int createdMountainPoints, int amountPoints, int xy);

	void draw(Mat& frame);

private:
	Point MountainPoints[1][500];
	int CreatedMountainPoints  = 0;
	int MountainHeight;
	int MountainLeft;
	int MountainRight;
	int MountainMiddle;
	int LeftPoints = 0;								// Amount of Points set left from the middle
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points
};