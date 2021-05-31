#pragma once
#pragma once

#include "Engine/Vector.h"
#include "Engine/Texture.h"

namespace UI
{
	extern Vector2 pixelsPerUnit;
	extern Vector2i winSize;

	/* Get pixel scaling and stuff for UI */
	extern void Initialize(const Vector2i& windowSize);

	/* Render GUI */
	extern void RenderUI(RenderTexture* dest);
}
