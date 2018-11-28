#include "opencv2/opencv.hpp"
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define GAMENAME	"Ballerburg "	
#define VERSION		"V0.1"	
#define WINHEIGHT	800	
#define WINWIDTH	1200	
#define ROCK_POINTS	50				// Amount of points between rock left - rock middle / rock middle - rock left

// Set colors
#define COLOR_BACKGROUND 250, 250, 250
#define COLOR_BALL 250, 0, 0

//#define DEBUG 18


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

	#ifdef DEBUG // LOG: Show calculation data
	printf("y:%d %d,%d x %d,%d \t restPoints=%d \tmaxRand=%d\n", xy, ax, ay, bx, by, restPoints, maxRand);
	#endif

	return maxRand; // return the max X or Y offset
}

// Draw the Rock
void DrawRock(Mat frame, int groundHeight) {

	int rockHeight = rand() % ((groundHeight - 100) - (winHeight / 8)) + (winHeight / 8);			// generate the height of the rock
	int rockLeft = rand() % (WINWIDTH / 3 - 200) + 200;											// generate start of the rock in 1/3 and 150px away from the border
	int rockRight = rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 200)) + ((WINWIDTH / 3 * 2) - 1);	// generate end of the rock in 3/3 and 150px away from the border
	int rockWidth = rockRight - rockLeft;			// Calc the width (X) of the rock
	int rockMiddle = (rockWidth / 2) + rockLeft;	// Calc the middle x cordinate of the rock
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points
	int leftPoints = 0;									// Amount of Points set left from the middle

	// Create the Rock
	Point rock_points[1][500];
	int counter = 0;
	rock_points[0][counter++] = Point(rockLeft, groundHeight); // add left ground point

	// Generate the points between left ground and middle height of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockMiddle, rockHeight, counter - 1, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockMiddle, rockHeight, counter - 1, ROCK_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint); // random offset widht max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint); // random offset widht max xDistancePerPoint

		if (!(counter - 10 >= ROCK_POINTS)) { // TODO: Use only one if
			if ((offsetx + rock_points[0][counter - 1].x > rockMiddle) || (rock_points[0][counter - 1].y - offsety < rockHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				// add new generated point
				rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety);
				counter++;
				leftPoints++;
			}
		}
		else {
			break; // Stop generating points if counter is "-10"
		}
	}

	rock_points[0][counter++] = Point(rockMiddle, rockHeight);

	// generate points between middle height and right bottom of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockRight, groundHeight, counter - 2 - leftPoints, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(rock_points[0][counter - 1].x, rock_points[0][counter - 1].y, rockRight, groundHeight, counter - 2 - leftPoints, ROCK_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint)-(yDistancePerPoint + 1);		// random offset with max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint);								// random offset with max xDistancePerPoint

		if (!(counter - 2 - leftPoints - 10 >= ROCK_POINTS)) { // TODO: Use only one if
			if ((offsetx + rock_points[0][counter - 1].x > rockRight) || (rock_points[0][counter - 1].y - offsety > groundHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				rock_points[0][counter] = Point(offsetx + rock_points[0][counter - 1].x, rock_points[0][counter - 1].y - offsety); // add new generated point
				counter++;	// add +1 to counter of points
			}
		}
		else {
			break; // Stop generating points if counter is "-10"
		}
	}


	rock_points[0][counter++] = Point(rockRight, groundHeight);		// add right ground point
	const Point* pRock[1] = { rock_points[0] };
	int numberOfPointsRock[] = { counter };							// number of points to draw
	fillPoly(frame, pRock, numberOfPointsRock, 1, Scalar(200, 200, 200), 8);	// draw the rock

	#ifdef DEBUG // LOG: 
	printf("groundHeight: %i\n", groundHeight);
	printf("Random rockHeight: %i\n", rockHeight);
	printf("Random rockLeft: %i\n", rockLeft);
	printf("Random rockRight: %i\n", rockRight);
	printf("Calc rockMiddle: %i\n", rockMiddle);
	printf("Nr. of points generated: %d\n", counter - 2);
	#endif	
}

void DrawCastle(Mat frame, int xPosition, int yPosition, bool turn) {

	Point castle[1][100]; // Init castle points
	int counter = 0;

	// set relative castle points
	castle[0][counter++] = Point(0, -90);
	castle[0][counter++] = Point(-5, -90);
	castle[0][counter++] = Point(15, -115);
	castle[0][counter++] = Point(35, -90);
	castle[0][counter++] = Point(30, -90);
	castle[0][counter++] = Point(30, -50);
	castle[0][counter++] = Point(40, -50);
	castle[0][counter++] = Point(40, -60);
	castle[0][counter++] = Point(50, -60);
	castle[0][counter++] = Point(50, -50);
	castle[0][counter++] = Point(60, -50);
	castle[0][counter++] = Point(60, -60);
	castle[0][counter++] = Point(70, -60);
	castle[0][counter++] = Point(70, -50);
	castle[0][counter++] = Point(80, -50);
	castle[0][counter++] = Point(80, -60);
	castle[0][counter++] = Point(90, -60);
	castle[0][counter++] = Point(90, -50);
	castle[0][counter++] = Point(100, -50);
	castle[0][counter++] = Point(100, -60);
	castle[0][counter++] = Point(110, -60);
	castle[0][counter++] = Point(110, -50);
	castle[0][counter++] = Point(120, -50);
	castle[0][counter++] = Point(120, -70);
	castle[0][counter++] = Point(115, -70);
	castle[0][counter++] = Point(115, -80);
	castle[0][counter++] = Point(120, -80);
	castle[0][counter++] = Point(120, -75);
	castle[0][counter++] = Point(125, -75);
	castle[0][counter++] = Point(125, -80);
	castle[0][counter++] = Point(130, -80);
	castle[0][counter++] = Point(130, -75);
	castle[0][counter++] = Point(135, -75);
	castle[0][counter++] = Point(135, -80);
	castle[0][counter++] = Point(140, -80);
	castle[0][counter++] = Point(140, -75);
	castle[0][counter++] = Point(145, -75);
	castle[0][counter++] = Point(145, -80);
	castle[0][counter++] = Point(150, -80);
	castle[0][counter++] = Point(150, -75);
	castle[0][counter++] = Point(155, -75);
	castle[0][counter++] = Point(155, -80);
	castle[0][counter++] = Point(160, -80);
	castle[0][counter++] = Point(160, -70);
	castle[0][counter++] = Point(155, -70);
	castle[0][counter++] = Point(155, -50);
	castle[0][counter++] = Point(155, 0);
	castle[0][counter++] = Point(0, 0);

	// mirror x coords
	if (turn) {
		for (int i = 0; i < counter; i++)
			castle[0][i].x *= -1;
	}

	// set absolute position of castle points
	for (int i = 0; i < counter; i++)
		castle[0][i] += Point(xPosition, yPosition);

	// Draw the Castle
	const Point* pCastle[1] = { castle[0] };
	int numberOfPointsCastle[] = { counter };
	fillPoly(frame, pCastle, numberOfPointsCastle, 1, Scalar(0, 0, 255), 8);
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

	Mat frame(winHeight, winWidth, CV_8UC3, Scalar(COLOR_BACKGROUND));

	// draw the ground
	for (int y = winHeight; y > groundHeight; y--) {
		for (int x = 0; x < winWidth; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(200, 200, 200), 1, 8, 0);
		}
	}

	// draw the rock
	DrawRock(frame, groundHeight);

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

	// Draw two castle
	DrawCastle(frame, 20, groundHeight, false);
	DrawCastle(frame, WINWIDTH - 20, groundHeight, true);

	imshow(windowName, frame);

	// Draw the cannonball
	// Mat, Point, Radius, Color, Dthickness, lineType, shift
	for (int x = 50; x <= WINWIDTH; x++) {
		int y = ((x - (WINWIDTH / 2)) * (x - (WINWIDTH / 2))) * 0.0031;

		#ifdef DEBUG
		printf("%dx%d\n", x, y);
		#endif	

		circle(frame, Point(x, y), 10, Scalar(COLOR_BALL), CV_FILLED, 8, 0);
		imshow(windowName, frame);
		#ifdef DEBUG
		
		#else
		waitKey(3);
		circle(frame, Point(x, y), 10, Scalar(COLOR_BACKGROUND), CV_FILLED, 8, 0);
		#endif
		imshow(windowName, frame);
	}


	// Der blaue Komenet
	int blauerkomet = rand() % 10000000;
	if (blauerkomet == 99) {
		for (int i = 0; i < WINWIDTH; i++) {
			circle(frame, Point(i, 100), 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
			imshow(windowName, frame);
			waitKey(33);
		}
	}	
	
	waitKey(0);

	// Close the Window
	destroyWindow(windowName);
}