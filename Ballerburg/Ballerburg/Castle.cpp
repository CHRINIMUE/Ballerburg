#pragma once

#include "Castle.h"

Castle::Castle(int xPosition, int yPosition, bool flip) {
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
	if (flip) {
		for (int i = 0; i < counter; i++)
			castle[0][i].x *= -1;
	}

	// set absolute position of castle points
	for (int i = 0; i < counter; i++)
		castle[0][i] += Point(xPosition, yPosition);
}

void Castle::draw(Mat& frame) {
	// Draw the Castle
	const Point* pCastle[1] = { castle[0] };
	int numberOfPointsCastle[] = { 48 };
	fillPoly(frame, pCastle, numberOfPointsCastle, 1, Scalar(COLOR_CASTLE), 8);
}