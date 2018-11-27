#include "opencv2/opencv.hpp"
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define GAMENAME	"Ballerburg "	
#define VERSION		"V0.1"	
#define WINHEIGHT	800	
#define WINWIDTH	1200	
#define ROCK_POINTS	50				// Amount of points between rock left - rock middle / rock middle - rock left

#define DEBUG 5


using namespace cv;
using namespace std;


String windowName = GAMENAME;
int winHeight = WINHEIGHT;
int winWidth = WINWIDTH;

// Calc the max disctance for random - 0 for x; 1 for y
int CalcDistance(int ax, int ay, int bx, int by, int counter, int amountPoints, int xy) {
	int restPoints = amountPoints - counter;	// Calc the remaining Points to generate
	int maxRand;

	if (restPoints > 1) { 
		if (xy == 0) { // x cordinate
			int distance = fabs(bx - ax);
			maxRand = distance / restPoints;
		}
		else { // y cordinate
			int distance = fabs(ay - by);
			maxRand = distance / (restPoints + 1);
		}
	}
	else {
		if (xy == 0) { // x cordinate
			int distance = fabs(bx - ax);
			maxRand = (distance  + 1) / 2;
			if (distance == 0)
				return 1;
		}
		else { // y cordinate
			int distance = fabs(ay - by);
			maxRand = (distance + 1) / 2;
			if (distance == 0)
				return 1;
		}
	}

	#ifdef DEBUG // LOG: Show the position of Point
	printf("y:%d %d,%d x %d,%d \t restPoints=%d \tmaxRand=%d\n", xy, ax, ay, bx, by, restPoints, maxRand);
	#endif

	return maxRand; // return the max X or Y offset
}

// Draw the Rock
void DrawRock(Mat frame, int groundHeight) {

	int rockHeight	= rand() % ((groundHeight - 100) - (winHeight / 8)) + (winHeight / 8);			// generate the height of the rock
	int rockLeft	= rand() % (WINWIDTH / 3 - 175) + 175;											// generate start of the rock in 1/3 and 150px away from the border
	int rockRight	= rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 175)) + ((WINWIDTH / 3 * 2) - 1);	// generate end of the rock in 3/3 and 150px away from the border
	int rockWidth	= rockRight - rockLeft;			// Calc the width (X) of the rock
	int rockMiddle	= (rockWidth / 2) + rockLeft;	// Calc the middle x cordinate of the rock
	int yDistance = groundHeight - rockHeight;		// Height of the rock
	int xDistance = rockMiddle - rockLeft;			// Widht of the rock (half)
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points

	// Create the Rock
	Point rock_points[1][500];
	int counter = 0;
	rock_points[0][counter++] = Point(rockLeft, groundHeight); // Left ground point

	// Generate the points between left ground and middle height of the rock
	while (1) {
		// Distance betweent left ground and middle height
		yDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockMiddle, rockHeight, counter - 1, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockMiddle, rockHeight, counter - 1, ROCK_POINTS, 0);

		// generate offset
		int offsety = rand() % (yDistancePerPoint);
		int offsetx = rand() % (xDistancePerPoint);

		if (!(counter - 10 >= ROCK_POINTS)) {
			if ((offsetx + rock_points[0][counter - 1].x > rockMiddle) || (rock_points[0][counter - 1].y - offsety < rockHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
				counter++;
			}
		}
		else {
			break; // Stop generating points if counter is "-10"
		}
	}

	rock_points[0][counter++] = Point(rockMiddle, rockHeight);
	
	// generate points between middle height and right bottom of the rock
	while (1) {
		// Distance betweent middle height and right ground
		yDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockRight, groundHeight, counter - 1 - ROCK_POINTS, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockRight, groundHeight, counter - 1 - ROCK_POINTS, ROCK_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint) - (yDistancePerPoint + 1);
		int offsetx = rand() % (xDistancePerPoint);


		if (!(counter - 10 >= ROCK_POINTS * 2)) {
			if ((offsetx + rock_points[0][counter - 1].x > rockRight) || (rock_points[0][counter - 1].y - offsety > groundHeight)) {
				break;
			}
			else {
				rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
				counter++;
			}
		}
		else {
			break; // Stop generating points if counter is "-10"
		}
	}

	rock_points[0][counter++] = Point(rockRight, groundHeight);
	rock_points[0][counter++] = Point(rockMiddle, groundHeight);
	rock_points[0][counter] = Point(rockLeft, groundHeight);
	const Point* pRock[1] = { rock_points[0] };
	int numberOfPointsRock[] = { counter };
	fillPoly(frame, pRock, numberOfPointsRock, 1, Scalar(200, 200, 200), 8);

	#ifdef DEBUG // For debug log:
	printf("groundHeight: %i\n", groundHeight);
	printf("Random rockHeight: %i\n", rockHeight);
	printf("Random rockLeft: %i\n", rockLeft);
	printf("Random rockRight: %i\n", rockRight);
	printf("Calc rockMiddle: %i\n", rockMiddle);
	printf("Nr. of points generated: %d\n", counter - 4);
	printf("Distance Y (height): %d\n", yDistance);
	printf("Distance X (widht): %d\n", xDistance);
	#endif	
}

int main() {
	//Init
	#ifdef DEBUG
	srand(DEBUG);
	#else
	srand(time(NULL));
	#endif

	// Main Variables
	int groundHeight	= winHeight - (winHeight / 8);		// generate the bottom ground
	int xPosition		= 20;								// X Position of castle
	int yPosition		= 700;								// Y Position of castle

	Mat frame(winHeight, winWidth, CV_8UC3, Scalar(250, 250, 250));

	// draw the ground
	for (int y = winHeight; y > groundHeight; y--) {
		for (int x = 0; x < winWidth; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(200, 200, 200), 1, 8, 0);
		}
	}

	// draw the rock
	DrawRock(frame, groundHeight);

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

	//draw the rock (BOX)
/*
for (int y = groundHeight; y > rockHeight; y--) {
	for (int x = rockLeft; x < rockRight; x++) {
		rectangle(frame, Rect(x, y, 1, 1), Scalar(0, 0, 255), 1, 8, 0);
	}
}*/


	namedWindow(windowName);
	imshow(windowName, frame);
	
	waitKey(0);

	// Close the Window
	destroyWindow(windowName);
}