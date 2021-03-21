#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector.h"

class Texture2D;
class RenderTexture;

namespace Renderer
{
	extern void DebugEnable();
	extern void Init(bool debugEnable);

	extern void ClearScreen(GLbitfield mask);
	extern void Viewport(const Vector2i& size);
	extern void Viewport(const Vector2i& size, const Vector2i& offset);
	extern void BindScreenBuffer();

	extern void Blit(Texture2D src, RenderTexture dest);
	extern void Blit(RenderTexture src, RenderTexture dest);
	extern void Blit(int src, int dest, const Vector2i& winSize);
	extern void BlitToScreen(Texture2D src, const Vector2i& winSize);
	extern void BlitToScreen(RenderTexture src, const Vector2i& winSize);

	extern void Release();
}