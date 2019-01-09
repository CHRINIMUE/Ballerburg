#include "opencv2/opencv.hpp"
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "defines.h"
#include "Castle.h"
#include "Mountain.h"


using namespace cv;
using namespace std;

String windowName = GAMENAME;
int winHeight = WINHEIGHT;
int winWidth = WINWIDTH;

// Calc the max disctance for random offset -> 0 for x; 1 for y


// Draw the Rock
void DrawRock(Mat frame) {
	
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
	//DrawRock(frame);
	Mountain m = Mountain();
	m.draw(frame);

	// Draw two castles
	Castle c1(20, groundHeight, false);
	Castle c2(WINWIDTH - 20, groundHeight, true);
	c1.draw(frame);
	c2.draw(frame);
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