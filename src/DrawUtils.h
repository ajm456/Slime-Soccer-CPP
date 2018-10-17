/*
 * DrawUtils.h
 *
 *  Created on: 10 Oct 2018
 *      Author: andrew
 */

//TODO: Utilise VBO and VAOs instead of immediate mode

#ifndef DRAWUTILS_H_
#define DRAWUTILS_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "GameParams.h"

namespace draw
{

void background()
{
	// Draw the sky
	glColor3f(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
	glBegin(GL_QUADS);
		glVertex2d(-1.0, 1.0);
		glVertex2d(1.0, 1.0);
		glVertex2d(1.0, GROUND_LEVEL);
		glVertex2d(-1.0, GROUND_LEVEL);
	glEnd();
	
	// Draw the ground
	glColor3f(GROUND_R, GROUND_G, GROUND_B);
	glBegin(GL_QUADS);
		glVertex2d(-1.0, GROUND_LEVEL);
		glVertex2d(1.0, GROUND_LEVEL);
		glVertex2d(1.0, -1.0);
		glVertex2d(-1.0, -1.0);
	glEnd();
	
	// Draw the foul area indicators
	glColor3f(GOAL_R, GOAL_G, GOAL_B);
	// Left
	glBegin(GL_QUADS);
		glVertex2d(-1.0, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF);
		glVertex2d(-1.0 + FOUL_AREA_WIDTH, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF);
		glVertex2d(-1.0 + FOUL_AREA_WIDTH, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF - FOUL_AREA_INDICATOR_HEIGHT);
		glVertex2d(-1.0, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF - FOUL_AREA_INDICATOR_HEIGHT);
	glEnd();
	// Right
	glBegin(GL_QUADS);
		glVertex2d(1.0 - FOUL_AREA_WIDTH, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF);
		glVertex2d(1.0, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF);
		glVertex2d(1.0, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF - FOUL_AREA_INDICATOR_HEIGHT);
		glVertex2d(1.0 - FOUL_AREA_WIDTH, GROUND_LEVEL - FOUL_AREA_INDICATOR_Y_DIFF - FOUL_AREA_INDICATOR_HEIGHT);
	glEnd();
}

void slime(int numBodySegments, int numEyeSegments, SlimeData *obj, BallData *ball)
{
	double x = obj->x;
	double y = obj->y;
	double width = obj->width;
	double height = obj->height;
	double red = obj->color[0];
	double green = obj->color[1];
	double blue = obj->color[2];
	double eyeY = y + SLIME_HEIGHT/2.0;
	double eyeX;
	if (obj->lookingRight)
	{
		eyeX = x + width/4.2;
	}
	else
	{
		eyeX = x - width/4.2;
	}
	std::vector<double> eyeToBall { ball->x - eyeX, ball->y - eyeY };
	double eyeToBallMag = std::sqrt(pow(eyeToBall[0], 2) + pow(eyeToBall[1], 2));
	std::vector<double> eyeToPupil { eyeToBall[0] * 0.35*EYE_RADIUS / eyeToBallMag, eyeToBall[1] * 0.35*EYE_RADIUS / eyeToBallMag };
	double pupilX = eyeX + eyeToPupil[0];
	double pupilY = eyeY + eyeToPupil[1];
	
	// Calculate the body vertex coordinates
	std::vector<double> xCoords(numBodySegments + 2);
	std::vector<double> yCoords(numBodySegments + 2);

	double dtheta = M_PI / numBodySegments;

	// Store the centre coordinate
	xCoords[0] = x;
	yCoords[0] = y;

	for (int n = 1; n < numBodySegments + 2; n++)
	{
		double theta = -M_PI / 2 + (n - 1) * dtheta;
		double vx = width / 2 * sin(theta) + x;
		double vy = height / 2 * cos(theta) + y;

		// Store the perimeter vertices
		xCoords[n] = vx;
		yCoords[n] = vy;
	}
	
	
	// Store the eye vertex data
	std::vector<double> eyeXCoords(numEyeSegments + 2);
	std::vector<double> eyeYCoords(numEyeSegments + 2);
	std::vector<double> pupilXCoords(numEyeSegments + 2);
	std::vector<double> pupilYCoords(numEyeSegments + 2);
	
	dtheta = 2*M_PI / numEyeSegments;
	
	// Store the centre coordinate
	eyeXCoords[0] = eyeX;
	eyeYCoords[0] = eyeY;
	pupilXCoords[0] = pupilX;
	pupilYCoords[0] = pupilY;
	
	for (int n = 1; n < numEyeSegments + 2; n++)
	{
		double theta = (n-1) * dtheta;
		double vx = eyeX + EYE_RADIUS * cos(theta);
		double vy = eyeY + EYE_RADIUS * ASPECT_RATIO * sin(theta);
		double pupilVx = pupilX + PUPIL_RADIUS_RATIO * EYE_RADIUS * cos(theta);
		double pupilVy = pupilY + PUPIL_RADIUS_RATIO * EYE_RADIUS * ASPECT_RATIO * sin(theta);
		
		// Store the perimeter vertices
		eyeXCoords[n] = vx;
		eyeYCoords[n] = vy;
		pupilXCoords[n] = pupilVx;
		pupilYCoords[n] = pupilVy;
	}
	
	
	// Draw the body data
	glColor3f(red, green, blue);
	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < numBodySegments + 2; i++)
		{
			glVertex2d(xCoords[i], yCoords[i]);
		}
	glEnd();
	
	// Draw the eye data
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < numEyeSegments + 2; i++)
		{
			glVertex2d(eyeXCoords[i], eyeYCoords[i]);
		}
	glEnd();
	
	// Draw the pupil eye data
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < numEyeSegments + 2; i++)
		{
			glVertex2d(pupilXCoords[i], pupilYCoords[i]);
		}
	glEnd();
}

void ball(int numSegments, BallData *obj)
{
	double x = obj->x;
	double y = obj->y;
	double width = obj->width;
	double height = obj->height;
	double red = BALL_R;
	double green = BALL_G;
	double blue = BALL_B;

	std::vector<double> *xCoords = new std::vector<double>(numSegments + 2);
	std::vector<double> *yCoords = new std::vector<double>(numSegments + 2);

	double dtheta = 2*M_PI / numSegments;

	// Store the centre coordinate
	xCoords->insert(xCoords->begin(), x);
	yCoords->insert(yCoords->begin(), y);

	for (int n = 1; n < numSegments + 2; n++)
	{
		double theta = -M_PI / 2 + (n - 1) * dtheta;
		double vx = width / 2 * sin(theta) + x;
		double vy = height / 2 * cos(theta) + y;

		// Store the perimeter vertices
		(*xCoords)[n] = vx;
		(*yCoords)[n] = vy;
	}

	// Draw the stored data
	glColor3f(red, green, blue);
	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < numSegments + 2; i++)
		{
			glVertex2f(xCoords->at(i), yCoords->at(i));
		}
	glEnd();
}

void circle(int numSegments, double x, double y, double radius, float red, float green,
		float blue)
{
	std::vector<double> xCoords(numSegments + 2);
	std::vector<double> yCoords(numSegments + 2);
	
	double dtheta = 2*M_PI / numSegments;
	
	// Store the centre coordinate
	xCoords[0] = x;
	yCoords[0] = y;
	
	for (int n = 1; n < numSegments + 2; n++)
	{
		double theta = (n - 1) * dtheta;
		double vx = radius / 2 * sin(theta) + x;
		double vy = (radius * ASPECT_RATIO) / 2 * cos(theta) + y;
		
		// Store the perimeter vertices
		xCoords[n] = vx;
		yCoords[n] = vy;
	}
	
	// Draw the stored data
	glColor3f(red, green, blue);
	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < numSegments + 2; i++)
		{
			glVertex2d(xCoords[i], yCoords[i]);
		}
	glEnd();
}

void rect(double x, double y, double width, double height, float red,
		float green, float blue)
{
	glColor3f(red, green, blue);
	glBegin (GL_QUADS);
		glVertex2d(x, y);
		glVertex2d(x + width, y);
		glVertex2d(x + width, y + height);
		glVertex2d(x, y + height);
	glEnd();
}

void goals()
{
	glColor3f(GOAL_R, GOAL_G, GOAL_B);
	// Left
	// Post
	glBegin(GL_QUADS);
		glVertex2d(-GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL);
		glVertex2d(-GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
		glVertex2d(-GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
		glVertex2d(-GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL);
	glEnd();
	// Crossbar
	glBegin(GL_QUADS);
		glVertex2d(-1.0, GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH/2.0);
		glVertex2d(-GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH/2.0);
		glVertex2d(-GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH/2.0);
		glVertex2d(-1.0, GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH/2.0);
	glEnd();
	// Vertical net
	double verticalNetGap = (1.0 - GOALPOST_RADIUS) / NUM_VERTICAL_NET_LINES;
	for (int n = 0; n < NUM_VERTICAL_NET_LINES; n++)
	{
		glBegin(GL_QUADS);
			glVertex2d(-1.0 + n*verticalNetGap - NET_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
			glVertex2d(-1.0 + n*verticalNetGap + NET_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
			glVertex2d(-1.0 + n*verticalNetGap + NET_WIDTH/2.0, GROUND_LEVEL);
			glVertex2d(-1.0 + n*verticalNetGap - NET_WIDTH/2.0, GROUND_LEVEL);
		glEnd();
	}
	// Horizontal net
	double horizontalNetGap = GOAL_HEIGHT / NUM_HORIZONTAL_NET_LINES;
	for (int n = 0; n < NUM_HORIZONTAL_NET_LINES; n++)
	{
		glBegin(GL_QUADS);
			glVertex2d(-1.0, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap + NET_WIDTH/2.0);
			glVertex2d(-GOALPOST_RADIUS, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap + NET_WIDTH/2.0);
			glVertex2d(-GOALPOST_RADIUS, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap - NET_WIDTH/2.0);
			glVertex2d(-1.0, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap - NET_WIDTH/2.0);
		glEnd();
	}
	
	// Right
	// Post
	glBegin(GL_QUADS);
		glVertex2d(GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL);
		glVertex2d(GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
		glVertex2d(GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
		glVertex2d(GOALPOST_RADIUS + GOALPOST_WIDTH/2.0, GROUND_LEVEL);
	glEnd();
	// Crossbar
	glBegin(GL_QUADS);
		glVertex2d(GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH/2.0);
		glVertex2d(1.0, GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH/2.0);
		glVertex2d(1.0, GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH/2.0);
		glVertex2d(GOALPOST_RADIUS - GOALPOST_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH/2.0);
	glEnd();
	// Vertical net
	for (int n = 1; n <= NUM_VERTICAL_NET_LINES; n++)
	{
		glBegin(GL_QUADS);
			glVertex2d(GOALPOST_RADIUS + n*verticalNetGap - NET_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
			glVertex2d(GOALPOST_RADIUS + n*verticalNetGap + NET_WIDTH/2.0, GROUND_LEVEL + GOAL_HEIGHT);
			glVertex2d(GOALPOST_RADIUS + n*verticalNetGap + NET_WIDTH/2.0, GROUND_LEVEL);
			glVertex2d(GOALPOST_RADIUS + n*verticalNetGap - NET_WIDTH/2.0, GROUND_LEVEL);
		glEnd();
	}
	// Horizontal net
	for (int n = 0; n < NUM_HORIZONTAL_NET_LINES; n++)
	{
		glBegin(GL_QUADS);
			glVertex2d(GOALPOST_RADIUS, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap + NET_WIDTH/2.0);
			glVertex2d(1.0, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap + NET_WIDTH/2.0);
			glVertex2d(1.0, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap - NET_WIDTH/2.0);
			glVertex2d(GOALPOST_RADIUS, GROUND_LEVEL + GOAL_HEIGHT - n*horizontalNetGap - NET_WIDTH/2.0);
		glEnd();
	}
}

}

#endif /* DRAWUTILS_H_ */
