#pragma once

#include "Vector.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Renderer
{
	extern void DebugEnable();
	extern void Init(bool debugEnable);

	extern void ClearScreen(GLbitfield mask);
	extern void Viewport(const Vector2i& size);
	extern void Viewport(const Vector2i& size, const Vector2i& offset);
	extern void BindScreenBuffer();

	extern void Blit(int src, int dest, const Vector2i& winSize);

	extern void Release();
}