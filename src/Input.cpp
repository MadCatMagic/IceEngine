#include "Input.h"
#include <iostream>

namespace Input
{
	int spaceState = 0;
	int lshiftState = 0;
	int wState = 0;
	int sState = 0;
	int dState = 0;
	int aState = 0;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			spaceState = action;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			lshiftState = action;
			break;
		case GLFW_KEY_W:
			wState = action;
			break;
		case GLFW_KEY_S:
			sState = action;
			break;
		case GLFW_KEY_D:
			dState = action;
			break;
		case GLFW_KEY_A:
			aState = action;
			break;
		}
	}

	bool GetKey(Key key)
	{
		return false;
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{

	}

	float cursorPosX = 0.0;
	float cursorPosY = 0.0;
	float diffX = 0.0f;
	float diffY = 0.0f;

	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		cursorPosX = (float)xpos;
		cursorPosY = (float)ypos;
	}

	void SetCursorType(CursorType type, GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, (int)type);
	}

	void EnableRawCursorData(GLFWwindow* window)
	{
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	void EnableInput(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, CursorPositionCallback);

		SetCursorType(CursorType::LOCKED, window);
		EnableRawCursorData(window);
	}
}