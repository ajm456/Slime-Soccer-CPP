/*
 * GameEngine.h
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include "GameData.h"
#include "WindowManager.h"
#include "PhysicsEngine.h"

class GameEngine
{
public:
	GameEngine();
	virtual ~GameEngine();
	void run();

private:
	WindowManager *m_wm;
	GameState *m_state;
	bool m_shouldClose;
	PhysicsEngine *m_physEng;

	void input();
	void physics(double dt);
	void graphics();
};

#endif /* GAMEENGINE_H_ */
