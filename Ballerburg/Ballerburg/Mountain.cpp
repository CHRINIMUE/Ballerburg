#pragma once

#include "Mountain.h"

Mountain::Mountain() {
	// Create the Rock
	mountainPoints[0][createdMountainPoints++] = Point(rockLeft, groundHeight); // add left ground point

	// Generate the points between left ground and middle height of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = CalcDistance(mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y, rockMiddle, rockHeight, createdMountainPoints - 1, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y, rockMiddle, rockHeight, createdMountainPoints - 1, ROCK_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint); // random offset widht max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint); // random offset widht max xDistancePerPoint

		if (!(createdMountainPoints - 10 >= ROCK_POINTS)) { // TODO: Use only one if
			if ((offsetx + mountainPoints[0][createdMountainPoints - 1].x > rockMiddle) || (mountainPoints[0][createdMountainPoints - 1].y - offsety < rockHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				// add new generated point
				mountainPoints[0][createdMountainPoints] = Point(offsetx + mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y - offsety);
				createdMountainPoints++;
				leftPoints++;
			}
		}
		else {
			break; // Stop generating points if createdMountainPoints is "-10"
		}
	}
	mountainPoints[0][createdMountainPoints++] = Point(rockMiddle, rockHeight);
	// generate points between middle height and right bottom of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = CalcDistance(mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y, rockRight, groundHeight, createdMountainPoints - 2 - leftPoints, ROCK_POINTS, 1);
		xDistancePerPoint = CalcDistance(mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y, rockRight, groundHeight, createdMountainPoints - 2 - leftPoints, ROCK_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint)-(yDistancePerPoint + 1);		// random offset with max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint);								// random offset with max xDistancePerPoint

		if (!(createdMountainPoints - 2 - leftPoints - 10 >= ROCK_POINTS)) { // TODO: Use only one if
			if ((offsetx + mountainPoints[0][createdMountainPoints - 1].x > rockRight) || (mountainPoints[0][createdMountainPoints - 1].y - offsety > groundHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				mountainPoints[0][createdMountainPoints] = Point(offsetx + mountainPoints[0][createdMountainPoints - 1].x, mountainPoints[0][createdMountainPoints - 1].y - offsety); // add new generated point
				createdMountainPoints++;	// add +1 to createdMountainPoints of points
			}
		}
		else {
			break; // Stop generating points if createdMountainPoints is "-10"
		}
	}

	mountainPoints[0][createdMountainPoints++] = Point(rockRight, groundHeight);	// add right ground point
	printf("%d\n", createdMountainPoints);
}

 int Mountain::CalcDistance(int ax, int ay, int bx, int by, int createdMountainPoints, int amountPoints, int xy) {
	int restPoints = amountPoints - createdMountainPoints;	// Calc the remaining Points to generate
	int maxRandom;

	if (restPoints > 1) {
		if (xy == 0) { // x cordinate
			int distance = fabs(bx - ax);
			maxRandom = distance / restPoints;
		}
		else { // y cordinate
			int distance = fabs(ay - by);
			maxRandom = distance / (restPoints + 1);
		}
	}
	else {
		if (xy == 0) { // x cordinate
			int distance = fabs(bx - ax);
			maxRandom = (distance + 1) / 2;
			if (distance == 0)
				return 1;
		}
		else { // y cordinate
			int distance = fabs(ay - by);
			maxRandom = (distance + 1) / 2;
			if (distance == 0)
				return 1;
		}
	}

	return maxRandom; // return the max X or Y offset
}

 void Mountain::draw(Mat& frame) {
	 // draw the ground
	 for (int y = WINWIDTH; y > groundHeight; y--) {
		 for (int x = 0; x < WINWIDTH; x++) {
			 rectangle(frame, Rect(x, y, 1, 1), Scalar(COLOR_ROCK), 1, 8, 0);
		 }
	 }
	 
	 // draw the mountain
	 const Point* pRock[1] = { mountainPoints[0] };
	 int numberOfPointsRock[] = { createdMountainPoints };	// number of points to draw
	 fillPoly(frame, pRock, numberOfPointsRock, 1, Scalar(COLOR_ROCK), 8);	// draw the rock
 }