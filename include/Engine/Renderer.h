#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector.h"

class Texture2D;
class RenderTexture;
class Camera;
class Light;
class Material;

namespace Renderer
{
	extern void DebugEnable();
	extern void Init(bool debugEnable, GLFWwindow* window);
	extern Vector2i GetWinSize();

	extern void SetMainCamera(Camera* cam);
	extern Camera* GetMainCamera();

	extern void ClearScreen(GLbitfield mask);
	extern void Viewport(const Vector2i& size);
	extern void Viewport(const Vector2i& size, const Vector2i& offset);
	extern void BindScreenBuffer();

	/* Blit functions */
	// using Texture2Ds
	extern void Blit(Texture2D src, RenderTexture dest);
	extern void Blit(Texture2D src, RenderTexture dest, Material* mat);
	extern void Blit(Texture2D src, Texture2D depth, RenderTexture dest);				 // includes depth texture
	extern void Blit(Texture2D src, Texture2D depth, RenderTexture dest, Material* mat); // includes depth texture
	// using RenderTextures
	extern void Blit(RenderTexture src, RenderTexture dest, bool drawDepth);
	extern void Blit(RenderTexture src, RenderTexture dest, Material* mat, bool drawDepth);
	// blitting onto RenderTexture id 0
	// depth ignored here because it wont matter
	extern void BlitToScreen(Texture2D src, const Vector2i& winSize);
	extern void BlitToScreen(RenderTexture src, const Vector2i& winSize);

	// render pipeline
	extern void RenderToCamera(Camera* cam);
	extern void PostProcess(Camera* cam, RenderTexture dest);

	extern void RenderGizmos(Camera* cam);

	extern void RenderLight(Light* light);

	// releases any heap variables
	extern void Release();
}