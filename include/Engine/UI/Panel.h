#pragma once

#include "Element.h"
#include "SpriteRenderer.h"

namespace UI {
	/* UI Panel - panel class, can display sprite */
	class Panel : public Element
	{
	public:
		Panel(const Vector2i& size, const Vector2i& pixelPos);
		Panel(const Vector2i& size, const Vector2i& pixelPos, Sprite* sprite);
		virtual ~Panel() override;

		virtual void Draw() override;
		virtual void OnTransform() override;
		virtual void HandleMouseClick(int action, Vector2i location) override;

		void SetSprite(Sprite* sprite);

	private:
		Vector2i size;
		SpriteRenderer* spriteRenderer{ nullptr };

		void CreateRenderer(Sprite* sprite);
	};
};