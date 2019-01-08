#pragma once

#include "defines.h"

#include "opencv2/opencv.hpp"
using namespace cv;

class Castle {
public:

	Castle(int xPosition, int yPosition, bool flip);

	void draw(Mat& frame);

private:
	Point castle[1][100];
};
