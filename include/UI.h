#pragma once
#include <vector>
#include <functional>
#include "Vector.h"

namespace UI
{
	Vector2 pixelsPerUnit = Vector2::one;

	/* Get pixel scaling and stuff for UI */
	extern void Initialize(Vector2i windowSize);

	/* UI Element - base class */
	class Element
	{
	public:
		inline Element() {}
		virtual ~Element() { for (auto& element : children) delete element; }

		virtual void HandleMouseClick(int action, Vector2i location) = 0;
		virtual void Draw() const = 0;

		void AssignChild(Element* child);

		inline Vector2i GetPixelPosition() const { return pixelPosition; }
		inline Vector2 GetPosition() const { return position; }
		inline Vector2 GetLocalPosition() const { return localPosition; }
		void SetPixelPosition(const Vector2i& pos);
		void SetPosition(const Vector2& pos);
		void SetLocalPosition(const Vector2& pos);

		inline Vector2 GetGlobalScale() const { return globalScale; }
		inline Vector2 GetLocalScale() const { return localScale; }
		void SetGlobalScale(const Vector2& scale);
		void SetLocalScale(const Vector2& scale);

	protected:
		Vector2i pixelPosition;
		Vector2 position;
		Vector2 localPosition;

		Vector2 globalScale;
		Vector2 localScale;

		void UpdateGlobalPosition();
		void UpdateGlobalScale();

		Element* parent = nullptr;

	private:
		std::vector<Element*> children;
	};

	/* UI Button - button class */
	class Button : public Element
	{
	public:
		inline Button(const Vector2i& size) { this->size = size; }
		virtual ~Button() {}

		virtual void HandleMouseClick(int action, Vector2i location) override;
		virtual void Draw() const override;

		void SetAction(std::function<void(int)> action);

	private:
		Vector2i size;

		std::function<void(int)> action = [](int button) {};
	};
}