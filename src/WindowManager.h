/*
 * WindowManager.h
 *
 *  Created on: 9 Oct 2018
 *      Author: andrew
 */

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include <GLFW/glfw3.h>

#include "GameData.h"

class WindowManager
{
public:
	WindowManager(int width, int height);
	virtual ~WindowManager();

	void draw(GameState *state);
	int wantsToClose();
	void setupInput();
	void pollEvents();
	static void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	static unsigned char keys;
	static bool resetRequest;

private:
	GLFWwindow* m_window;
};

#endif /* WINDOWMANAGER_H_ */
