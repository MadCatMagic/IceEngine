#include "UI/Button.h"
#include <iostream>

namespace UI
{
	Button::Button(const Vector2i& size, const Vector2i& pixelPos)
	{
		this->size = size;
		this->SetPixelPosition(pixelPos);
	}

	Button::Button(const Vector2i& size, const Vector2i& pixelPos, Sprite* sprite)
	{
		this->size = size;
		this->SetPixelPosition(pixelPos);
		CreateRenderer(sprite);
	}

	Button::~Button()
	{
		if (spriteRenderer != nullptr)
			delete spriteRenderer;
	}

	void Button::HandleMouseClick(int button, Vector2i location)
	{
		// if in bounds do stuff
		if (pixelPosition.x <= location.x && pixelPosition.x + size.x >= location.x && 
			pixelPosition.y <= location.y && pixelPosition.y + size.y >= location.y)
		{
			action(button);
		}
	}

	void Button::Draw()
	{
		spriteRenderer->Draw(position, Vector2(pixelPosition.x / pixelsPerUnit.x, pixelPosition.y / pixelsPerUnit.y));
	}

	void Button::OnTransform() { }

	void Button::SetAction(std::function<void(int)> action)
	{
		this->action = action;
	}

	void Button::SetSprite(Sprite* sprite)
	{
		CreateRenderer(sprite);
	}

	void Button::CreateRenderer(Sprite* sprite)
	{
		spriteRenderer = new SpriteRenderer(sprite);
	}
}
