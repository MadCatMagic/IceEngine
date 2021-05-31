#pragma once
#include "UICore.h"

#include <vector>

namespace UI {
	/* UI Element - base class */
	class Element
	{
	public:
		inline Element() { elements.push_back(this); }
		virtual ~Element() { for (auto& element : children) delete element; }

		virtual void HandleMouseClick(int action, Vector2i location) = 0;
		virtual void Draw() = 0;
		virtual void OnTransform() = 0;

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

		static void DrawElements();

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
		static std::vector<Element*> elements;
	};
}