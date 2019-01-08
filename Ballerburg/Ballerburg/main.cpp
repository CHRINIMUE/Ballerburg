#include "opencv2/opencv.hpp"
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "defines.h"
#include "Castle.h"


using namespace cv;
using namespace std;

String windowName = GAMENAME;
int winHeight = WINHEIGHT;
int winWidth = WINWIDTH;

// Calc the max disctance for random offset -> 0 for x; 1 for y
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
void DrawRock(Mat frame) {
	// Main Variables
	int groundHeight = GROUND_HEIGHT;	// generate the bottom ground
	int rockHeight = rand() % ((groundHeight - 100) - (winHeight / 4)) + (winHeight / 4);			// generate the height of the rock
	int rockLeft = rand() % (WINWIDTH / 3 - 200) + 200;												// generate start of the rock in 1/3 and 150px away from the border
	int rockRight = rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 200)) + ((WINWIDTH / 3 * 2) - 1);	// generate end of the rock in 3/3 and 150px away from the border
	int rockWidth = rockRight - rockLeft;			// Calc the width (X) of the rock
	int rockMiddle = (rockWidth / 2) + rockLeft;	// Calc the middle x cordinate of the rock
	int yDistancePerPoint;							// Max size between two Y points
	int xDistancePerPoint;							// Max size between two X points
	int leftPoints = 0;								// Amount of Points set left from the middle

	// draw the ground
	for (int y = winHeight; y > groundHeight; y--) {
		for (int x = 0; x < winWidth; x++) {
			rectangle(frame, Rect(x, y, 1, 1), Scalar(COLOR_ROCK), 1, 8, 0);
		}
	}

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
	fillPoly(frame, pRock, numberOfPointsRock, 1, Scalar(COLOR_ROCK), 8);	// draw the rock

#ifdef DEBUG // LOG: 
printf("groundHeight: %i\n", groundHeight);
printf("Random rockHeight: %i\n", rockHeight);
printf("Random rockLeft: %i\n", rockLeft);
printf("Random rockRight: %i\n", rockRight);
printf("Calc rockMiddle: %i\n", rockMiddle);
printf("Nr. of points generated: %d\n", counter - 2);
#endif	
}

void DrawCastle(Mat frame, int xPosition, int yPosition, bool flip) {
	//TODO: REMVOE	
}

/****************************************
Possible return values for CheckCollision:
0:	No Hit
1:	Hit Castle
2:	Hit Rock
3:	Outside the window (left/right/bottom)
4:	Outisde the window (top)
*****************************************/
int CheckCollision(int x, int y, Mat frame) {
	int checkResult = 0;
	double epsilon = M_PI / 8.0;
	for (double alpha = 0.0; alpha <= 2 * M_PI; alpha += epsilon) {
		double xPositionRadius = CANON_BALL_RADIUS * cos(alpha); // X offset of the circle
		double yPositionRadius = CANON_BALL_RADIUS * sin(alpha); // Y offset of the circle

		// Check hit for every point of the circle hitbox
		if ((x + xPositionRadius + 1 <= 0) || (x + xPositionRadius + 1 >= WINWIDTH) || ((int)(y + yPositionRadius + 1) >= WINHEIGHT)) {
			// Ball is outside the window (left/right/bottom)
			checkResult = 3;
			break;
		}
		else if ((int)(y + yPositionRadius + 1) <= 0){
			// Ball is outside the window (top) -> Ball should NOT stop
			checkResult = 4;
			break;
		}
		else if (frame.at<Vec3b>((int)(y + yPositionRadius + 1), (int)(x + xPositionRadius + 1)) == Vec3b(COLOR_CASTLE)) {
			// Hit with castle
			checkResult = 1;
			break;
		}
		else if (frame.at<Vec3b>((int)(y + yPositionRadius + 1), (int)(x + xPositionRadius + 1)) == Vec3b(COLOR_ROCK)) {
			// Hit with Rock
			checkResult = 2;
			break;
		}
	}

	#ifdef DEBUG // Only Debug stuff
	printf("Middle Hit Check Position: %i, %i\n", x, y);
	if (checkResult != -1) {
		printf("Hit TRUE\n");
		circle(frame, Point(x, y), CANON_BALL_RADIUS, Scalar(0, 0, 0), CV_FILLED, 8, 0);
		imshow(windowName, frame);
		waitKey(10);
	}
	else {
		printf("Hit FALSE\n");
	}
	#endif

	return checkResult;
}

void DrawBall(Mat frame, int x, int y) {
	// Mat, Point, Radius, Color, Dthickness, lineType, shift
	circle(frame, Point(x, y), CANON_BALL_RADIUS, Scalar(COLOR_BALL), CV_FILLED, 8, 0);
	imshow(windowName, frame);
	waitKey(10);
	circle(frame, Point(x, y), CANON_BALL_RADIUS + 1, Scalar(COLOR_BACKGROUND), CV_FILLED, 8, 0);

	#ifdef DEBUG
	printf("Canon Ball Position %i, %i\n", x, y);
	#endif
}

// Draw and calc the canon ball in Mat
void ShootCanonBall(Mat frame, bool turn, int* pulver, int* winkel) {
	// Position of Canon (Start of flying Ball)
	int xPos = 170;
	int yPos = 600;
	if (turn) {
		xPos = 1030;
		yPos = 600;
	}
	
	// Draw the cannonball
	int hitCounter = 0;
	int roundCounter = 0;
	int x;
	int y = yPos;
	float currentPulver = (int)*pulver;
	if (turn) { // Shot from right to left
		for (x = xPos; x > 1; x -= 2) {
			roundCounter++;
			if (roundCounter >= *winkel) {
				currentPulver -= 0.5;
				roundCounter = 0;
			}
			int distanceY = -currentPulver;
			int distanceX = fabs(x - xPos);
			y = y + distanceY;
			#ifdef DEBUG
			printf("%d\n", distanceY);
			#endif			
			DrawBall(frame, x, y);

			int hitResult = CheckCollision(x, y, frame);
			if (hitResult == 3) {
				#ifdef DEBUG
				printf("Ball outside of the Window - left/right/bottom\n");
				#endif			
				break;
			}
			else if (hitResult == 4) {
				#ifdef DEBUG
				printf("Ball outside of the Window - top\n");
				#endif
			}
			else if ((hitResult == 1) || (hitResult == 2)) {
				hitCounter++;
				if (hitCounter >= MAX_CANON_HITS) {
					hitCounter = 0;
					break;
				}
			}
		}
	}
	else { // Shoot from left to right
		for (x = xPos; x < WINWIDTH - 1; x += 2) {
			roundCounter++;
			if (roundCounter >= *winkel) {
				currentPulver -= 0.5;
				roundCounter = 0;
			}
			int distanceY = -currentPulver;
			int distanceX = fabs(x - xPos);
			y = y + distanceY;
			#ifdef DEBUG
			printf("%d\n", distanceY);
			#endif			
			DrawBall(frame, x, y);

			int hitResult = CheckCollision(x, y, frame);
			if (hitResult == 3) {
				#ifdef DEBUG
				printf("Ball outside of the Window - left/right/bottom\n");
				#endif			
				break;
			}
			else if (hitResult == 4) {
				#ifdef DEBUG
				printf("Ball outside of the Window - top\n");
				#endif
			}
			else if ((hitResult == 1) || (hitResult == 2)) {
				hitCounter++;
				if (hitCounter >= MAX_CANON_HITS) {
					hitCounter = 0;
					break;
				}
			}
		}
	}
}

void InitGame(Mat frame) {
	int groundHeight = GROUND_HEIGHT;
	// draw the rock
	DrawRock(frame);

	// Draw two castles
	Castle c1(20, groundHeight, false);
	Castle c2(WINWIDTH - 20, groundHeight, true);
	c1.draw(frame);
	c2.draw(frame);


	//DrawCastle(frame, 20, groundHeight, false);
	//DrawCastle(frame, WINWIDTH - 20, groundHeight, true);
}

// User input to change cannon settings
void UserInput(int** pulver, int** winkel, bool* exit) {
	int c = waitKeyEx(100);
	switch (c) {
		case 13: *exit = true; break;		// enter
		case 2490368: **pulver += 1; break; // cursor up
		case 2621440: **pulver -= 1; break; // cursor down
		case 2424832: **winkel += 1; break; // cursor left
		case 2555904: **winkel -= 1; break; // cursor right      
	}
}

// show user dialog to player 1 or 2
void UserDialog(bool turn, int* pulver, int* winkel) {
	bool exit = false;

	do {
		Mat user(200, 400, CV_8UC3, Scalar(COLOR_BACKGROUND));
		string pulverString;
		string winkelString;
		ostringstream convertPulver;
		ostringstream convertWinkel;
		string exitString = "Press ENTER to exit menu";
		string helpString = "Use the arrow keys to change settings.";

		// convert string and value of pulver/winkel
		convertPulver << "Pulver " << *pulver;
		pulverString = convertPulver.str();
		convertWinkel << "Winkel: " << *winkel;
		winkelString = convertWinkel.str();

		// print dialog text
		putText(user, pulverString, cvPoint(140, 30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(100, 100, 150), 1, CV_AA);
		putText(user, winkelString, cvPoint(140, 70), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(100, 100, 150), 1, CV_AA);
		putText(user, helpString, cvPoint(35, 120), FONT_HERSHEY_PLAIN, 1, cvScalar(100, 100, 150), 1, CV_AA);
		putText(user, exitString, cvPoint(40, 150), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(100, 100, 150), 1, CV_AA);
		
		// check user input
		UserInput(&pulver, &winkel, &exit);

		imshow("User Input", user);
		waitKey(1);
	} while (!exit);

	destroyWindow("User Input");
}

int main() {
	//Init
	#ifdef DEBUG
	srand(DEBUG);
	#else
	srand(time(NULL));
	#endif

	// Init the playground frame
	Mat frame(winHeight, winWidth, CV_8UC3, Scalar(COLOR_BACKGROUND));

	// Init the playground
	InitGame(frame);

	// Der blaue Komenet TODO: Move in extra function
	int blauerkomet = rand() % 1000000;
	if (blauerkomet == 10) {
		for (int i = 0; i < WINWIDTH + 30; i++) {
			circle(frame, Point(i, 70), 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
			imshow(windowName, frame);
			waitKey(10);
			circle(frame, Point(i - 30, 70), 10, Scalar(COLOR_BACKGROUND), CV_FILLED, 8, 0);
		}
	}

	int pulver1 = 5;
	int pulver2 = 5;
	int winkel1 = 20;
	int winkel2 = 20;

	bool play = true;
	bool user2 = true;
	int round = 1;
	do {
		imshow(windowName, frame);
		if (user2) {
			user2 = false;
			UserDialog(user2, &pulver2, &winkel2);
			ShootCanonBall(frame, user2, &pulver2, &winkel2);
		}
		else {
			user2 = true;
			UserDialog(user2, &pulver1, &winkel1);
			ShootCanonBall(frame, user2, &pulver1, &winkel1);
		}
		round++;
		//TODO: Add Game exit
	} while (play);

	waitKey(0);
	
	// Close the Window
	destroyWindow(windowName);
}