#include "UI.h"
#include <iostream>

namespace UI
{
    namespace
    {
        static bool textureSetup = false;
        static RenderTexture* renderTexture;
        static Texture2D* renderedTexture;
    }
    Vector2 pixelsPerUnit = Vector2::one;
    Vector2i winSize = Vector2i::zero;

	void Initialize(const Vector2i& windowSize)
	{
		pixelsPerUnit.x = (float)(windowSize.x / 2.0f);
		pixelsPerUnit.y = (float)(windowSize.y / 2.0f);
        
        winSize = windowSize;

        SpriteRenderer::SetupRenderer();
        TextRenderer::SetupRenderer();
	}

    void RenderUI(RenderTexture* dest)
    {
        // Render to renderTexture
        dest->Bind();
        Renderer::Viewport(winSize);
        Element::DrawElements();
    }
}