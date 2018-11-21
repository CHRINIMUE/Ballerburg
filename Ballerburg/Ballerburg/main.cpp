#include "opencv2/opencv.hpp"
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define GAMENAME "Ballerburg "
#define VERSION "V0.1"
#define WINHEIGHT 800
#define WINWIDTH 1200

using namespace cv;
using namespace std;


int main()
{
	//Init
	srand(time(NULL));
	//srand(4);

	// Main Variables
	String windowName = GAMENAME;
	int winHeight = WINHEIGHT;
	int winWidth = WINWIDTH;
	int groundHeight = winHeight - (winHeight / 8); //generate the bottom ground
	int rockHeight = rand() % (groundHeight + (winHeight / 8)) + (winHeight / 8); //generate the max height of the rock (should be between groundHeight=700 and 200)
	int rockLeft = rand() % WINWIDTH/3; //generat start of the rock in 1/3
	int rockRight = rand() % ((WINWIDTH )- (WINWIDTH/3*2))+ (WINWIDTH / 3 * 2); //generate start of the rock in 3/3
	int rockWidth = rockRight - rockLeft;
	int rockMiddle = (rockWidth/2) + rockLeft;

	Mat frame(winHeight, winWidth, CV_8UC3, Scalar(250, 250, 250));
	

	// draw the ground
	for (int y = winHeight; y > groundHeight; y--) {
		for (int x = 0; x < winWidth; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(255, 0, 0), 1, 8, 0);
		}
	}
	//draw the rock (BOX)
	/*
	for (int y = groundHeight; y > rockHeight; y--) {
		for (int x = rockLeft; x < rockRight; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(0, 0, 255), 1, 8, 0);
		}
	}*/

	// Log:
	printf("groundHeight: %i\n", groundHeight);
	printf("Random rockHeight: %i\n", rockHeight);
	printf("Random rockLeft: %i\n", rockLeft);
	printf("Random rockRight: %i\n", rockRight);
	printf("Random rockMiddle: %i\n", rockMiddle);

	// Create Points for F
	Point rock_points[1][500];
	int counter = 0;
	rock_points[0][counter++] = Point(rockLeft, groundHeight);
	int yDistance = groundHeight - rockHeight;
	int yDistanceProStep = yDistance / 25;
	int xDistance = rockMiddle - rockLeft;
	int xDistanceProStep = xDistance / 25;
	while (1) { //generate dynamic rock left from middle
		int offsetx = rand() % (xDistanceProStep*2);
		int offsety = rand() % (yDistanceProStep*2);
		if ((offsetx + rock_points[0][counter - 1].x > rockMiddle) || (rock_points[0][counter - 1].y - offsety < rockHeight)) {
			break; // stop crating points, if reached the middle or the rockHeight
		}
		else {
			rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
			counter++;
		}
	}
	rock_points[0][counter++] = Point(rockMiddle, rockHeight);
	while (1) { // generate dynamic rock right form frock
		int offsetx = rand() % (xDistanceProStep * 2);
		int offsety = rand() % (yDistanceProStep * 2) - (yDistanceProStep * 2);
		if ((offsetx + rock_points[0][counter - 1].x > rockRight) || (rock_points[0][counter - 1].y - offsety > groundHeight)) {
			break;
		}
		else {
			rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
			counter++;
		}
	}
	rock_points[0][counter++] = Point(rockRight, groundHeight);
	rock_points[0][counter++] = Point(rockMiddle, groundHeight);
	rock_points[0][counter]   = Point(rockLeft, groundHeight);
	const Point* ppt[1] = { rock_points[0] };
	int npt[] = { counter };
	fillPoly(frame, ppt, npt, 1, Scalar(0, 255, 0), 8);

	//Log:
	printf("Nr of points generated: %d\n", counter);
	printf("Max X per step: %d\n", xDistanceProStep);
	printf("Max Y per step: %d\n", yDistanceProStep);


	// Later: read, remove and add specific pixel 
	/*
	for (int y=200; y<500; y++)
		for (int x=200; x<=300; x++)
			// set pixel
			frame.at<Vec3b>(Point(x, y)) = Vec3b(0,0,255); 

	Vec3b color = frame.at<Vec3b>(Point(300, 200));
	*/
	/*
	rectangle(frame, Rect(20, 20, 40, 60), Scalar(255, 0, 0), 1, 8, 0);
	rectangle(frame, Rect(100, 100, 40, 60), Scalar(255, 0, 0), 1, 8, 0);
	rectangle(frame, Rect(200, 200, 40, 60), Scalar(255, 0, 0), 1, 8, 0);
	*/

	namedWindow(windowName);
	imshow(windowName, frame);
	
	waitKey(0);

	// Close the Window
	destroyWindow(windowName);
}