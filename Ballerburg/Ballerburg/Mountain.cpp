#pragma once

#include "Mountain.h"

Mountain::Mountain(int MinHeight) {
	//MountainHeight = rand() % ((GROUND_HEIGHT - 100) - (WINHEIGHT / 8)) + (WINHEIGHT / 8);
	MountainHeight = rand() % MinHeight;
	MountainLeft = rand() % (WINWIDTH / 3 - 200) + 200;
	MountainRight = rand() % (WINWIDTH - ((WINWIDTH / 3 * 2) + 200)) + ((WINWIDTH / 3 * 2) - 1);
	MountainMiddle = ((MountainRight - MountainLeft) / 2) + MountainLeft;

	MountainPoints[0][CreatedMountainPoints++] = Point(MountainLeft, GROUND_HEIGHT); // add left ground point

	// Generate the points between left ground and middle height of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = calcDistance(MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y, MountainMiddle, MountainHeight, CreatedMountainPoints - 1, MOUNTAIN_POINTS, 1);
		xDistancePerPoint = calcDistance(MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y, MountainMiddle, MountainHeight, CreatedMountainPoints - 1, MOUNTAIN_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint); // random offset widht max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint); // random offset widht max xDistancePerPoint

		if (!(CreatedMountainPoints - 10 >= MOUNTAIN_POINTS)) { // TODO: Use only one if
			if ((offsetx + MountainPoints[0][CreatedMountainPoints - 1].x > MountainMiddle) || (MountainPoints[0][CreatedMountainPoints - 1].y - offsety < MountainHeight)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				// add new generated point
				MountainPoints[0][CreatedMountainPoints] = Point(offsetx + MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y - offsety);
				CreatedMountainPoints++;
				LeftPoints++;
			}
		}
		else {
			break; // Stop generating points if createdMountainPoints is "-10"
		}
	}
	MountainPoints[0][CreatedMountainPoints++] = Point(MountainMiddle, MountainHeight);
	// generate points between middle height and right bottom of the rock
	while (1) {
		// calc max distance for X and Y
		yDistancePerPoint = calcDistance(MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y, MountainRight, GROUND_HEIGHT, CreatedMountainPoints - 2 - LeftPoints, MOUNTAIN_POINTS, 1);
		xDistancePerPoint = calcDistance(MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y, MountainRight, GROUND_HEIGHT, CreatedMountainPoints - 2 - LeftPoints, MOUNTAIN_POINTS, 0);

		int offsety = rand() % (yDistancePerPoint)-(yDistancePerPoint + 1);		// random offset with max yDistancePerPoint
		int offsetx = rand() % (xDistancePerPoint);								// random offset with max xDistancePerPoint

		if (!(CreatedMountainPoints - 2 - LeftPoints - 10 >= MOUNTAIN_POINTS)) { // TODO: Use only one if
			if ((offsetx + MountainPoints[0][CreatedMountainPoints - 1].x > MountainRight) || (MountainPoints[0][CreatedMountainPoints - 1].y - offsety > GROUND_HEIGHT)) {
				break; // stop crating points, if reached the middle or the rockHeight
			}
			else {
				MountainPoints[0][CreatedMountainPoints] = Point(offsetx + MountainPoints[0][CreatedMountainPoints - 1].x, MountainPoints[0][CreatedMountainPoints - 1].y - offsety); // add new generated point
				CreatedMountainPoints++;	// add +1 to createdMountainPoints of points
			}
		}
		else {
			break; // Stop generating points if createdMountainPoints is "-10"
		}
	}

	MountainPoints[0][CreatedMountainPoints++] = Point(MountainRight, GROUND_HEIGHT);	// add right ground point
}

// Calc the max disctance for random offset -> 0 for x; 1 for y
 int Mountain::calcDistance(int ax, int ay, int bx, int by, int createdMountainPoints, int amountPoints, int xy) {
	int restPoints = amountPoints - createdMountainPoints;	// Calc the remaining Points to generate
	int maxRandom;

	if (restPoints > 1) {
		if (xy == 0) { // x cordinate
			int distance = (int)fabs(bx - ax);
			maxRandom = distance / restPoints;
		}
		else { // y cordinate
			int distance = (int)fabs(ay - by);
			maxRandom = distance / (restPoints + 1);
		}
	}
	else {
		if (xy == 0) { // x cordinate
			int distance = (int)fabs(bx - ax);
			maxRandom = (distance + 1) / 2;
			if (distance == 0)
				return 1;
		}
		else { // y cordinate
			int distance = (int)fabs(ay - by);
			maxRandom = (distance + 1) / 2;
			if (distance == 0)
				return 1;
		}
	}

	return maxRandom; // return the max X or Y offset
}

 void Mountain::draw(Mat& frame) {
	 // draw the ground
	 for (int y = WINWIDTH; y > GROUND_HEIGHT; y--) {
		 for (int x = 0; x < WINWIDTH; x++) {
			 rectangle(frame, Rect(x, y, 1, 1), Scalar(COLOR_ROCK), 1, 8, 0);
		 }
	 }
	 
	 // draw the mountain
	 const Point* pMountain[1] = { MountainPoints[0] };
	 int NumberOfPointsRock[] = { CreatedMountainPoints };	// number of points to draw
	 fillPoly(frame, pMountain, NumberOfPointsRock, 1, Scalar(COLOR_ROCK), 8);	// draw the rock
 }