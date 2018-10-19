/*
 * GameState.h
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include "GameParams.h"
#include <vector>

struct SlimeData
{
	double x, y;
	double velX, velY;
	double width, height;
	float color[3];
	bool grounded, preJump, lookingRight;
	unsigned char movement;

	SlimeData(double x, double y, float red, float green, float blue, bool lookingRight) : x(x), y(y), lookingRight(lookingRight)
	{
		velX = 0.0;
		velY = 0.0;
		width = SLIME_WIDTH;
		height = SLIME_HEIGHT;
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		grounded = true;
		preJump = false;
		movement = 0;
	}
};
typedef struct SlimeData SlimeData;

struct BallData
{
	double x, y, prevX, prevY;
	double velX, velY;
	double width, height;
	
	BallData(double x, double y) : x(x), y(y)
	{
		prevX = 0.0;
		prevY = 0.0;
		velX = 0.0;
		velY = 0.0;
		width = BALL_RADIUS*2.0;
		height = BALL_RADIUS*2.0;
	}
};

enum GameMode
{
	play, foul, goal
};

struct GameState
{
	GameMode mode;
	std::vector<SlimeData*> *slimes;
	BallData *ball;
	double leftFoulTime, rightFoulTime;
	float leftTeamColor[3], rightTeamColor[3];
	int leftTeamScore, rightTeamScore;

	GameState(std::vector<SlimeData*> *slimes, BallData *ball, std::vector<float> leftColor, std::vector<float> rightColor) : slimes(slimes), ball(ball)
	{
		mode = play;
		leftFoulTime = FOUL_DURATION_MAX;
		rightFoulTime = FOUL_DURATION_MAX;
		leftTeamColor[0] = leftColor[0];
		leftTeamColor[1] = leftColor[1];
		leftTeamColor[2] = leftColor[2];
		rightTeamColor[0] = rightColor[0];
		rightTeamColor[1] = rightColor[1];
		rightTeamColor[2] = rightColor[2];
		leftTeamScore = 0;
		rightTeamScore = 0;
	}
};
typedef struct GameState GameState;

#endif /* GAMEDATA_H_ */
