#pragma once
#include <functional>

#include "Element.h"
#include "SpriteRenderer.h"

namespace UI {
	/* UI Button - button class */
	class Button : public Element
	{
	public:
		Button(const Vector2i& size, const Vector2i& pixelPos);
		Button(const Vector2i& size, const Vector2i& pixelPos, Sprite* sprite);
		virtual ~Button();

		virtual void HandleMouseClick(int action, Vector2i location) override;
		virtual void Draw() override;
		virtual void OnTransform() override;

		void SetAction(std::function<void(int)> action);
		void SetSprite(Sprite* sprite);

	private:
		Vector2i size;
		SpriteRenderer* spriteRenderer{ nullptr };

		std::function<void(int)> action = [](int button) {};

		void CreateRenderer(Sprite* sprite);
	};
};