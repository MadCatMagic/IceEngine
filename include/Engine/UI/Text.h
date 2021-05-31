#pragma once
#include "TextRenderer.h"
#include "Element.h"

namespace UI {
	class Text : public Element
	{
	public:
		Text(unsigned int size, const Vector2i& pixelPos);
		Text(unsigned int size, const Vector2i& pixelPos, const std::string& text);
		Text(unsigned int size, const Vector2i& pixelPos, const std::string& text, const std::string& filepath);
		virtual ~Text();

		inline virtual void HandleMouseClick(int action, Vector2i location) override {};
		virtual void Draw() override;
		virtual void OnTransform() override;

		void SetText(const std::string& text);
		void SetFont(const std::string& filepath);

		void CreateRenderer();

	private:
		int size;
		std::string text;
		std::string font;
		TextRenderer* textRenderer{ nullptr };
	};
};