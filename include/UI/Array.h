#pragma once
#include "UI/Element.h"

namespace UI
{
	/* UI Button - button class */
	class Array : public Element
	{
	public:
		Array(const Vector2i& offset, const Vector2i& pixelPos);
		Array(const Vector2i& offset, const Vector2i& pixelPos, unsigned int gridSize);
		virtual ~Array();

		inline virtual void HandleMouseClick(int action, Vector2i location) override {};
		virtual void Draw() override;
		virtual void OnTransform() override;

	private:
		Vector2i offset;
		unsigned int gridSize;
	};
}
