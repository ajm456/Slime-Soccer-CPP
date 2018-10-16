/*
 * GameEngine.cpp
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#include "GameEngine.h"
#include "GameData.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include "GameParams.h"
#include <cmath>
#include <chrono>
#include <algorithm>
#include <thread>

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::run()
{
	std::cout << "Initialising game..." << std::endl;
	init();

	// Set time used for timestep calculation
	using clock = std::chrono::high_resolution_clock;
	auto currentTime = clock::now();
	
	std::vector<double> fpsSamples(NUM_FPS_SAMPLES, 1.0);
	int fpsIndex = 0;
			
	std::cout << "Beginning game loop..." << std::endl;
	while (!m_shouldClose)
	{	
		auto newTime = clock::now();
		double frameTime = std::chrono::duration_cast<std::chrono::duration<double>>(newTime - currentTime).count();
		currentTime = newTime;
		
		input();
		
		physics(frameTime);
		
		graphics();
		
		fpsSamples[fpsIndex] = frameTime;
		if(++fpsIndex > NUM_FPS_SAMPLES - 1)
		{
			fpsIndex = 0;
		}
		double fpsSum = 0;
		for(double fps : fpsSamples)
		{
			fpsSum += 1.0/fps;
		}
		
		if(m_state->m_mode == play)
		{
			printf("FPS: %f\n", fpsSum/NUM_FPS_SAMPLES);
			printf("Score: %i-%i\n\n", m_state->m_leftTeamScore, m_state->m_rightTeamScore);
		}
	}

	std::cout << "Gracefully exiting..." << std::endl;
}

void GameEngine::init()
{
	// Set the random seed
	std::srand(time(0));
	
	// Load and shuffle colour list to determine team colours
	std::vector<std::vector<float>> colorList(COLOR_LIST);
	std::random_shuffle(colorList.begin(), colorList.end());
	std::vector<float> leftColor = colorList[0];
	std::vector<float> rightColor = colorList[1];
	
	
	// Initialise the window manager
	m_wm = new WindowManager(WINDOW_HEIGHT * ASPECT_RATIO, WINDOW_HEIGHT);

	// Initialise the initial game state
	std::vector<SlimeData*> *slimeData = new std::vector<SlimeData*>();
	SlimeData *slime1 = new SlimeData(-SLIME_INIT_X_RADIUS, SLIME_INIT_Y, leftColor[0], leftColor[1], leftColor[2], true);
	SlimeData *slime2 = new SlimeData(SLIME_INIT_X_RADIUS, SLIME_INIT_Y, rightColor[0], rightColor[1], rightColor[2], false);
	slimeData->push_back(slime1);
	slimeData->push_back(slime2);

	BallData *ball = new BallData(BALL_INIT_X, BALL_INIT_Y);

	m_state = new GameState(slimeData, ball, leftColor, rightColor);

	// Initialise the physics engine
	m_physEng = new PhysicsEngine(m_state);

	// Set the keyboard callbacks for input
	m_wm->setupInput();
}

void GameEngine::input()
{
	m_wm->pollEvents();

	// Check to see if any window events suggest the window should close i.e.
	// the X button has been pressed
	m_shouldClose = m_wm->wantsToClose();

	// Take the 6-bit WindowManager keys bitmask and pass its parts to the
	// player slimes
	unsigned char keys = m_wm->keys;
	m_state->m_slimes->at(0)->m_movement = keys & 0x7;
	m_state->m_slimes->at(1)->m_movement = (keys & 0x38) >> 3;
	
	// Check whether a mode reset key has been pressed in the WindowManager and
	// pass this information to the PhysicsEngine
	m_physEng->setResetFlag(m_wm->resetRequest);
}

void GameEngine::physics(double dt)
{
	m_physEng->process(dt);
}

void GameEngine::graphics()
{
	m_wm->draw(m_state);
}

