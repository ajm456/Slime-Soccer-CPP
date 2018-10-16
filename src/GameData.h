/*
 * GameState.h
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include <vector>
#include "GameParams.h"

struct CircularObjectData
{
	double m_x, m_y;
	double m_velX, m_velY;
	double m_width, m_height;
	float m_color[3];

	CircularObjectData(double x, double y, double width, double height, double angle,
			float red, float green, float blue)
	{
		m_x = x;
		m_y = y;
		m_velX = 0.0;
		m_velY = 0.0;
		m_width = width;
		m_height = height;
		m_color[0] = red;
		m_color[1] = green;
		m_color[2] = blue;
	}
};
typedef struct CircularObjectData CircularObjectData;

struct SlimeData: CircularObjectData
{
	bool m_grounded, m_preJump, m_lookingRight;
	unsigned char m_movement;

	SlimeData(double x, double y, double red, double green, double blue, bool lookingRight) :
			CircularObjectData(x, y, SLIME_WIDTH, SLIME_HEIGHT*ASPECT_RATIO, 180.0, red,
					green, blue)
	{
		m_grounded = true;
		m_preJump = false;
		m_movement = 0;
		m_lookingRight = lookingRight;
	}
};
typedef struct SlimeData SlimeData;

struct BallData: CircularObjectData
{
	double m_prevX, m_prevY;
	
	BallData(double x, double y) :
			CircularObjectData(x, y, 2*BALL_RADIUS, 2*BALL_RADIUS*ASPECT_RATIO, 360.0, BALL_R,
					BALL_G, BALL_B)
	{
		m_prevX = 0.0;
		m_prevY = 0.0;
	}
};

enum GameMode
{
	play, foul, goal
};

struct GameState
{
	GameMode m_mode;
	std::vector<SlimeData*> *m_slimes;
	BallData *m_ball;
	double m_leftFoulTime, m_rightFoulTime;
	float m_leftTeamColor[3], m_rightTeamColor[3];
	int m_leftTeamScore, m_rightTeamScore;

	GameState(std::vector<SlimeData*> *slimes, BallData *ball, std::vector<float> leftColor, std::vector<float> rightColor)
	{
		m_mode = play;
		m_slimes = slimes;
		m_ball = ball;
		m_leftFoulTime = FOUL_DURATION_MAX;
		m_rightFoulTime = FOUL_DURATION_MAX;
		m_leftTeamColor[0] = leftColor[0];
		m_leftTeamColor[1] = leftColor[1];
		m_leftTeamColor[2] = leftColor[2];
		m_rightTeamColor[0] = rightColor[0];
		m_rightTeamColor[1] = rightColor[1];
		m_rightTeamColor[2] = rightColor[2];
		m_leftTeamScore = 0;
		m_rightTeamScore = 0;
	}
};
typedef struct GameState GameState;

#endif /* GAMEDATA_H_ */
