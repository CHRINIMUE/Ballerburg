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
	//srand(6);

	// Main Variables
	String windowName = GAMENAME;
	int winHeight = WINHEIGHT;
	int winWidth = WINWIDTH;
	int groundHeight = winHeight - (winHeight / 8); //generate the bottom ground
	int rockHeight = rand() % ((groundHeight - 100) - (winHeight/4)) + (winHeight/4);
	int rockLeft = rand() % WINWIDTH/3; //generat start of the rock in 1/3
	int rockRight = rand() % ((WINWIDTH )- (WINWIDTH/3*2))+ (WINWIDTH / 3 * 2); //generate start of the rock in 3/3
	int rockWidth = rockRight - rockLeft;
	int rockMiddle = (rockWidth/2) + rockLeft;

	Mat frame(winHeight, winWidth, CV_8UC3, Scalar(250, 250, 250));
	
	// draw the ground
	for (int y = winHeight; y > groundHeight; y--) {
		for (int x = 0; x < winWidth; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(200, 200, 200), 1, 8, 0);
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
	printf("Calc rockMiddle: %i\n", rockMiddle);

	// Variables for rock generator
	int yDistance			= groundHeight - rockHeight;		// Height of the rock
	int xDistance			= rockMiddle - rockLeft;			// Widht of the rock (half)
	int maxSize				= 7;								// Average of distance between two points
	int xyMinPoints;

	if (xDistance > yDistance) 
		xyMinPoints = xDistance / maxSize;						// Widht > Height -> Min. width points
	else 
		xyMinPoints = yDistance / maxSize;						// Height > Width -> Min. height points

	int yDistancePerPoint	= yDistance / xyMinPoints;			// Max size between two points
	int xDistancePerPoint	= xDistance / xyMinPoints;			// Max size between two points

	// Create the Rock
	Point rock_points[1][500];
	int counter = 0;
	rock_points[0][counter++] = Point(rockLeft, groundHeight);
	//generate dynamic rock left from middle
	while (1) { 
		int offsety = rand() % (yDistancePerPoint * 2);
		int offsetx = rand() % (xDistancePerPoint * 2);
		if ((offsetx + rock_points[0][counter - 1].x > rockMiddle) || (rock_points[0][counter - 1].y - offsety < rockHeight)) {
			break; // stop crating points, if reached the middle or the rockHeight
		}
		else {
			rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
			counter++;
		}
	}
	rock_points[0][counter++] = Point(rockMiddle, rockHeight);
	// generate dynamic rock right from middle
	while (1) { 
		int offsety = rand() % (yDistancePerPoint * 2) - (yDistancePerPoint * 2);
		int offsetx = rand() % (xDistancePerPoint * 2);
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
	const Point* pRock[1] = { rock_points[0] };
	int numberOfPointsRock[] = { counter };
	fillPoly(frame, pRock, numberOfPointsRock, 1, Scalar(200, 200, 200), 8);

	//Log:
	printf("Nr. of points generated: %d\n", counter);
	printf("Distance Y (height): %d\n", yDistance);
	printf("Distance X (widht): %d\n", xDistance);
	printf("Min. Points: %d\n", xyMinPoints);
	printf("Max X per step: %d\n", xDistancePerPoint);
	printf("Max Y per step: %d\n", yDistancePerPoint);


	int xPosition = 20;
	int yPosition = 700;

	// Draw the Castle
	Point castle[1][100];
	castle[0][0] = Point(xPosition + 0, yPosition + (-100));
	castle[0][1] = Point(xPosition + 15, yPosition + (-125));
	castle[0][2] = Point(xPosition + 30, yPosition + (-100));
	castle[0][3] = Point(xPosition + 30, yPosition + (-50));
	castle[0][4] = Point(xPosition + 40, yPosition + (-50));
	castle[0][5] = Point(xPosition + 40, yPosition + (-60));
	castle[0][6] = Point(xPosition + 50, yPosition + (-60));
	castle[0][7] = Point(xPosition + 50, yPosition + (-50));
	castle[0][8] = Point(xPosition + 60, yPosition + (-50));
	castle[0][9] = Point(xPosition + 60, yPosition + (-60));
	castle[0][10] = Point(xPosition + 70, yPosition + (-60));
	castle[0][11] = Point(xPosition + 70, yPosition + (-50));
	castle[0][12] = Point(xPosition + 80, yPosition + (-50));
	castle[0][13] = Point(xPosition + 80, yPosition + (-60));
	castle[0][14] = Point(xPosition + 90, yPosition + (-60));
	castle[0][15] = Point(xPosition + 90, yPosition + (-50));
	castle[0][16] = Point(xPosition + 100, yPosition + (-50));
	castle[0][17] = Point(xPosition + 100, yPosition + (-60));
	castle[0][18] = Point(xPosition + 110, yPosition + (-60));
	castle[0][19] = Point(xPosition + 110, yPosition + (-50));
	castle[0][20] = Point(xPosition + 120, yPosition + (-50));
	castle[0][21] = Point(xPosition + 120, yPosition + (-70));
	castle[0][22] = Point(xPosition + 115, yPosition + (-70));
	castle[0][23] = Point(xPosition + 115, yPosition + (-80));
	castle[0][24] = Point(xPosition + 120, yPosition + (-80));
	castle[0][25] = Point(xPosition + 120, yPosition + (-75));
	castle[0][26] = Point(xPosition + 125, yPosition + (-75));
	castle[0][27] = Point(xPosition + 125, yPosition + (-80));
	castle[0][28] = Point(xPosition + 130, yPosition + (-80));
	castle[0][29] = Point(xPosition + 130, yPosition + (-75));
	castle[0][30] = Point(xPosition + 135, yPosition + (-75));
	castle[0][31] = Point(xPosition + 135, yPosition + (-80));
	castle[0][32] = Point(xPosition + 140, yPosition + (-80));
	castle[0][33] = Point(xPosition + 140, yPosition + (-75));
	castle[0][34] = Point(xPosition + 145, yPosition + (-75));
	castle[0][35] = Point(xPosition + 145, yPosition + (-80));
	castle[0][36] = Point(xPosition + 150, yPosition + (-80));
	castle[0][37] = Point(xPosition + 150, yPosition + (-75));
	castle[0][38] = Point(xPosition + 155, yPosition + (-75));
	castle[0][39] = Point(xPosition + 155, yPosition + (-80));
	castle[0][40] = Point(xPosition + 160, yPosition + (-80));
	castle[0][41] = Point(xPosition + 160, yPosition + (-70));
	castle[0][42] = Point(xPosition + 155, yPosition + (-70));
	castle[0][43] = Point(xPosition + 155, yPosition + (-50));
	castle[0][44] = Point(xPosition + 155, yPosition + 0);
	castle[0][45] = Point(xPosition + 0, yPosition + 0);
	const Point* pCastle[1] = { castle[0] };
	int numberOfPointsCastle[] = { 46 };
	fillPoly(frame, pCastle, numberOfPointsCastle, 1, Scalar(0, 0, 255), 8);

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