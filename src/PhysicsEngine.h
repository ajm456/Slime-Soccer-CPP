/*
 * PhysicsEngine.h
 *
 *  Created on: 10 Oct 2018
 *      Author: ajm456
 */

#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

#include "GameData.h"

class PhysicsEngine
{
public:
	PhysicsEngine(GameState *state);
	virtual ~PhysicsEngine();
	void process(double dt);
	void setResetFlag(bool resetFlag);
	
	bool m_physicsAccel;

private:
	GameMode *m_mode;
	std::vector<SlimeData*> *m_slimes;
	BallData *m_ball;
	double *m_leftFoulTime, *m_rightFoulTime;
	int *m_leftTeamScore, *m_rightTeamScore;
	bool m_resetFlag;

	void processSlimes(double dt);
	void processBall(double dt);
	void processCollisions(double dt);
	void processGoals();
	void checkFouls(double dt);
	void processFouls();
	void reset();
};

#endif /* PHYSICSENGINE_H_ */
