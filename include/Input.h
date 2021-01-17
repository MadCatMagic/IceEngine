#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Input
{
	// keyboard
	enum class Key {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		SPACE, LSHIFT, RSHIFT, LCONTROL, RCONTROL
	};
	bool GetKey(Key key);

	extern int spaceState;
	extern int lshiftState;
	extern int wState;
	extern int sState;
	extern int dState;
	extern int aState;

	// mouse
	enum class CursorType {
		NORMAL = GLFW_CURSOR_NORMAL, HIDDEN = GLFW_CURSOR_HIDDEN, LOCKED = GLFW_CURSOR_DISABLED
	};

	void SetCursorType(CursorType type, GLFWwindow* window);
	void EnableRawCursorData(GLFWwindow* window);
	
	extern double cursorPosX;
	extern double cursorPosY;

	// all
	void EnableInput(GLFWwindow* window);
};
