/*
 * GameParams.h
 *
 *  Created on: 10 Oct 2018
 *      Author: andrew
 */

#ifndef GAMEPARAMS_H_
#define GAMEPARAMS_H_

#include <vector>

/* Window params */
const double ASPECT_RATIO = 1.78;
const int WINDOW_HEIGHT = 900;

/* Background params */
const float BACKGROUND_R = 0.0f;
const float BACKGROUND_G = 0.0f;
const float BACKGROUND_B = 0.7f;
const float GROUND_R = 0.4f;
const float GROUND_G = 0.4f;
const float GROUND_B = 0.4f;

/* Slime params */
const double SLIME_INIT_Y = -0.7;
const double SLIME_INIT_X_RADIUS = 0.6; // the distance from 0.0 to the outermost slimes
const double SLIME_WIDTH = 0.2;
const double SLIME_HEIGHT = 0.18;
const double JUMP_VEL = 2.0;
const double HORIZ_VEL = 1.0;
const double EYE_RADIUS = 0.02;
const double PUPIL_RADIUS_RATIO = 0.45;

/* Ball params */
const double BALL_INIT_X = 0.0;
const double BALL_INIT_Y = 0.9;
const double BALL_RADIUS = 0.03;
const float BALL_R = 0.8f;
const float BALL_G = 0.8f;
const float BALL_B = 0.0f;
const double INDICATOR_Y = 0.8;
const double INDICATOR_SCALE = 0.1;
const float INDICATOR_R = 0.4f;
const float INDICATOR_G = 0.4f;
const float INDICATOR_B = 0.4f;

/* Goal & foul params */
const double FOUL_AREA_WIDTH = 0.25;
const double FOUL_AREA_INDICATOR_Y_DIFF = 0.05;
const double FOUL_AREA_INDICATOR_HEIGHT = 0.01;
const double FOUL_DURATION_MAX = 1.0;
const double FOULBAR_Y_DIFF = 0.01;
const double FOULBAR_HEIGHT = 0.03;
const double GOALPOST_WIDTH = 0.015;
const double GOAL_HEIGHT = 0.6;
const double GOALPOST_RADIUS = 0.8; // the distance from 0.0 to the goal post
const int NUM_VERTICAL_NET_LINES = 12;
const int NUM_HORIZONTAL_NET_LINES = 20;
const double NET_WIDTH = 0.004;
const float GOAL_R = 1.0f;
const float GOAL_G = 1.0f;
const float GOAL_B = 1.0f;

/* Physics params */
const double GRAVITY = -4.81;
const double GROUND_LEVEL = -0.7;
const double GROUND_BALL_RESTI = 0.70;
const double SLIME_BALL_RESTI = 0.60;
const double HORIZ_CROSSBAR_HBOX_HEIGHT = 0.05;

/* Key params */
const unsigned short KEY_W = 1;
const unsigned short KEY_A = 2;
const unsigned short KEY_D = 4;
const unsigned short KEY_UP = 8;
const unsigned short KEY_LEFT = 16;
const unsigned short KEY_RIGHT = 32;
const unsigned short SLIME_JUMP = 1;
const unsigned short SLIME_LEFT = 2;
const unsigned short SLIME_RIGHT = 4;

/* Misc params */
const int NUM_PLAYERS = 2;
const int NUM_FPS_SAMPLES = 10;
const std::vector<std::vector<float>> COLOR_LIST = {
		/* red */ 			{ 1.0f, 0.0f, 0.0f },
		/* green */			{ 0.0f, 1.0f, 0.0f },
		/* pink */			{ 1.0f, 0.55f, 0.85f },
		/* turquoise */ 	{ 0.4f, 0.9f, 0.8f},
		/* black */			{ 0.0f, 0.0f, 0.0f },
		/* light grey */	{ 0.9f, 0.9f, 0.9f }
};

#endif /* GAMEPARAMS_H_ */
