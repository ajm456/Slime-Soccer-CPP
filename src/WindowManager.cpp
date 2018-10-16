/*
 * WindowManager.cpp
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#include "WindowManager.h"
#include <iostream>
#include "GameData.h"
#include <cmath>
#include <stdio.h>
#include "DrawUtils.h"
#include "GameParams.h"


unsigned char WindowManager::keys = 0;
bool WindowManager::resetRequest = false;

WindowManager::WindowManager(int width, int height)
{
	// Initialise the library
	if (!glfwInit())
		std::cout << "FUG :DD" << std::endl;

	// Create a windowed mode window and its OpenGL context
	m_window = glfwCreateWindow(width, height,
			"Slime Soccer 2: Electric Boogaloo", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
	}

	// Make the window's context current
	glfwMakeContextCurrent(m_window);
}

WindowManager::~WindowManager()
{
	glfwTerminate();
}

void WindowManager::draw(GameState *state)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Draw the background
	draw::background();

	// Draw the ball
	int numBallSegments = 50;
	draw::ball(numBallSegments, state->m_ball);

	// Draw the slimes
	std::vector<SlimeData*> *slimes = state->m_slimes;
	int numSlimeSegments = 15;
	int numEyeSegments = 15;
	for (SlimeData *slime : *slimes)
	{
		draw::slime(numSlimeSegments, numEyeSegments, slime, state->m_ball);
	}

	// Draw the ball indicator if required
	int numIndicatorSegments = 50;
	if (state->m_ball->m_y > 1.0)
	{
		double indicatorRadius = (state->m_ball->m_y - 1.0) * INDICATOR_SCALE;
		draw::circle(numIndicatorSegments, state->m_ball->m_x, INDICATOR_Y,
				indicatorRadius, INDICATOR_R, INDICATOR_G, INDICATOR_B);
	}
	
	// Draw the foul bars
	// Left
	draw::rect(-1.0, GROUND_LEVEL - FOULBAR_Y_DIFF - FOULBAR_HEIGHT, state->m_leftFoulTime / FOUL_DURATION_MAX, FOULBAR_HEIGHT, state->m_leftTeamColor[0], state->m_leftTeamColor[1], state->m_leftTeamColor[2]);
	// Right
	draw::rect(FOUL_DURATION_MAX - state->m_rightFoulTime, GROUND_LEVEL - FOULBAR_Y_DIFF - FOULBAR_HEIGHT, state->m_rightFoulTime / FOUL_DURATION_MAX, FOULBAR_HEIGHT, state->m_rightTeamColor[0], state->m_rightTeamColor[1], state->m_rightTeamColor[2]);
	
	// Draw the goals
	draw::goals();
	
	glfwSwapBuffers(m_window);
}

void WindowManager::pollEvents()
{
	glfwPollEvents();
}

int WindowManager::wantsToClose()
{
	return glfwWindowShouldClose(m_window);
}

void WindowManager::setupInput()
{
	keys = 0;
	glfwSetKeyCallback(m_window, processInput);
}

void WindowManager::processInput(GLFWwindow* window, int key, int scancode,
		int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			keys |= KEY_W;
			break;
		case GLFW_KEY_A:
			keys |= KEY_A;
			break;
		case GLFW_KEY_D:
			keys |= KEY_D;
			break;
		case GLFW_KEY_UP:
			keys |= KEY_UP;
			break;
		case GLFW_KEY_LEFT:
			keys |= KEY_LEFT;
			break;
		case GLFW_KEY_RIGHT:
			keys |= KEY_RIGHT;
			break;
		case GLFW_KEY_ENTER:
			resetRequest = true;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			keys = keys & (~KEY_W);
			break;
		case GLFW_KEY_A:
			keys = keys & (~KEY_A);
			break;
		case GLFW_KEY_D:
			keys = keys & (~KEY_D);
			break;
		case GLFW_KEY_UP:
			keys = keys & (~KEY_UP);
			break;
		case GLFW_KEY_LEFT:
			keys = keys & (~KEY_LEFT);
			break;
		case GLFW_KEY_RIGHT:
			keys = keys & (~KEY_RIGHT);
			break;
		case GLFW_KEY_ENTER:
			resetRequest = false;
			break;
		default:
			break;
		}
	}
}

