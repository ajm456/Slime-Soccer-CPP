/*
 * PhysicsEngine.cpp
 *
 *  Created on: 10 Oct 2018
 *      Author: ajm456
 */

#include "PhysicsEngine.h"
#include "GameParams.h"
#include <stdio.h>
#include <iostream>
#include <cmath>

PhysicsEngine::PhysicsEngine(GameState *state)
{
	m_mode = &(state->m_mode);
	m_slimes = state->m_slimes;
	m_ball = state->m_ball;
	m_leftFoulTime = &(state->m_leftFoulTime);
	m_rightFoulTime = &(state->m_rightFoulTime);
	m_leftTeamScore = &(state->m_leftTeamScore);
	m_rightTeamScore = &(state->m_rightTeamScore);
	m_resetFlag = false;
	m_physicsAccel = true;
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::process(double dt)
{
	if (*m_mode == play)
	{
		processGoals();
	}
	// Check mode again as processGoals() is capable of changing it
	if (*m_mode == play)
	{
		processSlimes(dt);
		processBall(dt);
		processCollisions(dt);
		checkFouls(dt);
		processFouls();
		printf("The ball is at (%f, %f)\n", m_ball->m_x, m_ball->m_y);
	}
	else if (m_resetFlag)
	{
		reset();
	}
}

void PhysicsEngine::processSlimes(double dt)
{
	for (SlimeData *slime : *m_slimes)
	{
		if ((slime->m_movement & SLIME_JUMP) == SLIME_JUMP && slime->m_grounded)
		{
			slime->m_preJump = true;
		}

		if ((slime->m_movement & SLIME_LEFT) == SLIME_LEFT
				&& (slime->m_movement & SLIME_RIGHT) != SLIME_RIGHT)
		{
			slime->m_velX = -HORIZ_VEL;
		}
		else if ((slime->m_movement & SLIME_RIGHT) == SLIME_RIGHT
				&& (slime->m_movement & SLIME_LEFT) != SLIME_LEFT)
		{
			slime->m_velX = HORIZ_VEL;
		}
		else
		{
			slime->m_velX = 0.0;
		}

		// Check if slime has become grounded
		if (slime->m_y <= GROUND_LEVEL && !slime->m_preJump)
		{
			slime->m_y = GROUND_LEVEL;
			slime->m_velY = 0.0;
			slime->m_grounded = true;
		}

		// If ungrounded, apply gravity
		if (!slime->m_grounded)
		{
			slime->m_velY += GRAVITY * dt;
		}

		// Check if slime wants to jump
		if (slime->m_preJump && slime->m_grounded)
		{
			slime->m_grounded = false;
			slime->m_velY = JUMP_VEL;
			slime->m_preJump = false;
		}

		// Move according to velocity
		slime->m_x += slime->m_velX * dt;
		slime->m_y += slime->m_velY * dt;
	}
}

void PhysicsEngine::processBall(double dt)
{
	// Bounce if hitting the ground
	if (m_ball->m_y - m_ball->m_height / 2 <= GROUND_LEVEL)
	{
		m_ball->m_velY = -m_ball->m_velY * GROUND_BALL_RESTI;
		m_ball->m_y = GROUND_LEVEL + m_ball->m_height / 2.0
				- (m_ball->m_velY * dt) + 0.001;
	}
	// Bounce if hitting left crossbar from side
	else if (m_ball->m_x - m_ball->m_width / 2.0 < -GOALPOST_RADIUS
			&& m_ball->m_prevX - m_ball->m_width / 2.0 > -GOALPOST_RADIUS
			&& m_ball->m_y - m_ball->m_height / 2.0
					< GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH / 2.0
			&& m_ball->m_y + m_ball->m_height / 2.0
					> GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH / 2.0)
	{
		m_ball->m_velX = -m_ball->m_velX * GROUND_BALL_RESTI;
		m_ball->m_x = -GOALPOST_RADIUS + m_ball->m_width / 2.0
				- (m_ball->m_velX * dt) + 0.001;
	}
	// Bounce if hitting right crossbar from side
	else if (m_ball->m_x + m_ball->m_width / 2.0 > GOALPOST_RADIUS
			&& m_ball->m_prevX + m_ball->m_width / 2.0 < GOALPOST_RADIUS
			&& m_ball->m_y - m_ball->m_height / 2.0
					< GROUND_LEVEL + GOAL_HEIGHT + GOALPOST_WIDTH / 2.0
			&& m_ball->m_y + m_ball->m_height / 2.0
					> GROUND_LEVEL + GOAL_HEIGHT - GOALPOST_WIDTH / 2.0)
	{
		m_ball->m_velX = -m_ball->m_velX * GROUND_BALL_RESTI;
		m_ball->m_x = GOALPOST_RADIUS - m_ball->m_width / 2.0
				+ (m_ball->m_velX * dt) - 0.001;
	}
	// Bounce if hitting crossbar from top
	else if (m_ball->m_prevY - m_ball->m_height / 2.0
			> GROUND_LEVEL + GOAL_HEIGHT
			&& m_ball->m_y - m_ball->m_height / 2.0
					<= GROUND_LEVEL + GOAL_HEIGHT
			&& (m_ball->m_x - m_ball->m_width / 2.0 < -GOALPOST_RADIUS
					|| m_ball->m_x + m_ball->m_width / 2.0 > GOALPOST_RADIUS))
	{
		m_ball->m_velY = -m_ball->m_velY * GROUND_BALL_RESTI;
		m_ball->m_y = GROUND_LEVEL + GOAL_HEIGHT + m_ball->m_height / 2.0
				- (m_ball->m_velY * dt) + 0.001;
	}
	// Bounce if hitting crossbar from bottom
	else if (m_ball->m_prevY + m_ball->m_height / 2.0
			< GROUND_LEVEL + GOAL_HEIGHT
			&& m_ball->m_y + m_ball->m_height / 2.0 > GROUND_LEVEL + GOAL_HEIGHT
			&& (m_ball->m_x - m_ball->m_width / 2.0 < -GOALPOST_RADIUS
					|| m_ball->m_x + m_ball->m_width / 2.0 > GOALPOST_RADIUS))
	{
		m_ball->m_velY = -m_ball->m_velY * GROUND_BALL_RESTI;
		m_ball->m_y = GROUND_LEVEL + GOAL_HEIGHT - m_ball->m_height / 2.0
				+ (m_ball->m_velY * dt) + 0.001;
	}
	// Bounce if hitting left wall
	else if (m_ball->m_x - m_ball->m_width / 2 <= -1.0)
	{
		m_ball->m_velX = -m_ball->m_velX * GROUND_BALL_RESTI;
		m_ball->m_x = -1.0 + m_ball->m_width / 2 - (m_ball->m_velX * dt)
				+ 0.001;
	}
	// Bounce if hitting right wall
	else if (m_ball->m_x + m_ball->m_width / 2 >= 1.0)
	{
		m_ball->m_velX = -m_ball->m_velX * GROUND_BALL_RESTI;
		m_ball->m_x = 1.0 - m_ball->m_width / 2 + (m_ball->m_velX * dt) - 0.001;
	}
	else
	{
		// Accelerate ball according to gravity
		m_ball->m_velY += GRAVITY * dt;
	}

	// Store the ball's current position for collision detection next frame
	m_ball->m_prevX = m_ball->m_x;
	m_ball->m_prevY = m_ball->m_y;

	// Change ball position according to velocity
	m_ball->m_x += m_ball->m_velX * dt;
	m_ball->m_y += m_ball->m_velY * dt;
}

void PhysicsEngine::processCollisions(double dt)
{
	for (SlimeData *slime : *m_slimes)
	{
		// Calculate the radius of the slime to use
		// Get the angle from positive x-axis on slime to ball's centre
		// Get vector from slime centre to ball centre
		std::vector<double> v
		{ m_ball->m_x - slime->m_x, m_ball->m_y - slime->m_y };

		// If ball is below the slime, there can be no collision
		if (v[1] < -m_ball->m_height)
		{
			continue;
		}

		// Use atan2 to calculate radian angle between
		double theta = std::atan2(v[1], v[0]);

		// Convert result of atan2 to positive
		if (theta < 0)
		{
			theta += 2 * M_PI;
		}

		// Get the radius on the slime pointing towards the ball
		double a = slime->m_width / 2.0;
		double b = slime->m_height / 2.0;
		double radius = a * b
				/ std::sqrt(
						pow(a, 2) * pow(sin(theta), 2)
								+ pow(b, 2) * pow(cos(theta), 2));

		// If the slime has jumped on top of the ball, pop it up above with
		// zero velocity
		if (m_ball->m_x > slime->m_x - slime->m_width / 2
				&& m_ball->m_x < slime->m_x + slime->m_width / 2 && v[1] < 0)
		{
			m_ball->m_y = slime->m_y + slime->m_height;
			m_ball->m_velX = 0;
			m_ball->m_velY = 0;
			continue;
		}

		// Check if the distance between ball and slime's centre is less than radius
		if (pow(v[0], 2) + pow(v[1], 2)
				<= pow(radius + m_ball->m_width / 2.0, 2))
		{
			// Calculate the ball's current speed
			double ballSpeed = std::sqrt(
					pow(m_ball->m_velX, 2) + pow(m_ball->m_velY, 2));

			// Get the magnitude of v
			double vMag = std::sqrt(pow(v[0], 2) + pow(v[1], 2));

			// Get the new velocity vector
			std::vector<double> newVel
			{ v[0] * (ballSpeed / vMag), v[1] * (ballSpeed / vMag) };

			// Scale according to the coefficient of restitution
			newVel[0] *= SLIME_BALL_RESTI;
			newVel[1] *= SLIME_BALL_RESTI;

			std::vector<double> pushOut
			{ v[0] * (radius + m_ball->m_width / 2.0 + 0.001) / vMag, v[1]
					* (radius + m_ball->m_width / 2.0 + 0.001) / vMag };

			m_ball->m_x = slime->m_x + pushOut[0];
			m_ball->m_y = slime->m_y + pushOut[1];

			// Add the velocity of the slime to the new ball velocity
			newVel[0] += slime->m_velX
					* std::abs(v[0] / (slime->m_width / 2.0));
			newVel[1] += slime->m_velY
					* std::abs(v[1] / (slime->m_height / 2.0));

			// Set the new ball velocity
			m_ball->m_velX = newVel[0];
			m_ball->m_velY = newVel[1];
		}
	}
}

void PhysicsEngine::processGoals()
{
	// Check for goal
	// Left
	if (m_ball->m_x + m_ball->m_width / 2.0 < -GOALPOST_RADIUS
			&& m_ball->m_y + m_ball->m_height / 2.0
					< GROUND_LEVEL + GOAL_HEIGHT)
	{
		*m_mode = goal;
		*m_rightTeamScore += 1;
	}
	// Right
	else if (m_ball->m_x - m_ball->m_width / 2.0 > GOALPOST_RADIUS
			&& m_ball->m_y + m_ball->m_height / 2.0
					< GROUND_LEVEL + GOAL_HEIGHT)
	{
		*m_mode = goal;
		*m_leftTeamScore += 1;
	}
}

void PhysicsEngine::checkFouls(double dt)
{
	for (SlimeData *slime : *m_slimes)
	{
		// Store the team the slime is on, based on the direction its looking
		int team = slime->m_lookingRight ? 1 : 2;

		if (team == 1)
		{
			// If left team is fouling
			if (slime->m_x < -1.0 + FOUL_AREA_WIDTH)
			{
				*m_leftFoulTime -= dt;
			}
			else if (*m_leftFoulTime < FOUL_DURATION_MAX)
			{
				// If they were previously fouling but are no longer, reset
				// their foul timer
				*m_leftFoulTime = FOUL_DURATION_MAX;
			}
		}
		else // team == 2
		{
			// If right team is fouling
			if (slime->m_x > 1.0 - FOUL_AREA_WIDTH)
			{
				*m_rightFoulTime -= dt;
			}
			else if (*m_rightFoulTime < FOUL_DURATION_MAX)
			{
				// If they were previously fouling but are no longer, reset
				// their foul timer
				*m_rightFoulTime = FOUL_DURATION_MAX;
			}
		}
	}
}

void PhysicsEngine::processFouls()
{
	if (*m_leftFoulTime <= 0)
	{
		*m_mode = foul;
		*m_leftTeamScore = std::min(0, *m_leftTeamScore - 1);
	}
	else if (*m_rightFoulTime <= 0)
	{
		*m_mode = foul;
		*m_rightTeamScore = std::min(0, *m_rightTeamScore - 1);
	}
}

void PhysicsEngine::setResetFlag(bool resetFlag)
{
	m_resetFlag = resetFlag;
}

void PhysicsEngine::reset()
{
	if (NUM_PLAYERS == 2)
	{
		(*m_slimes)[0]->m_x = -SLIME_INIT_X_RADIUS;
		(*m_slimes)[0]->m_y = SLIME_INIT_Y;
		(*m_slimes)[0]->m_velX = 0.0;
		(*m_slimes)[0]->m_velY = 0.0;
		(*m_slimes)[1]->m_x = SLIME_INIT_X_RADIUS;
		(*m_slimes)[1]->m_y = SLIME_INIT_Y;
		(*m_slimes)[1]->m_velX = 0.0;
		(*m_slimes)[1]->m_velY = 0.0;
	}

	m_ball->m_x = BALL_INIT_X;
	m_ball->m_y = BALL_INIT_Y;
	m_ball->m_velX = 0.0;
	m_ball->m_velY = 0.0;

	*m_leftFoulTime = FOUL_DURATION_MAX;
	*m_rightFoulTime = FOUL_DURATION_MAX;

	*m_mode = play;

}
