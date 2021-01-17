#pragma once

#include "Vector.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Renderer
{
	void DebugEnable();
	void Init(bool debugEnable);

	void ClearScreen(GLbitfield mask);
	void Viewport(const Vector2i& size);
	void Viewport(const Vector2i& size, const Vector2i& offset);

	void Blit(int src, int dest, const Vector2i& winSize);

	void Release();
}