#include "UI.h"
#include <iostream>

namespace UI
{
	void Element::HandleMouseClick(int action, Vector2i location) { }

	void Element::Draw() { }

	void Element::AssignChild(Element* child)
	{
		children.push_back(child);
		child->parent = this;
	}

	void Element::SetPixelPosition(const Vector2i& pos)
	{
		pixelPosition = pos;
		position.x = pos.x / pixelsPerUnit.x;
		position.y = pos.y / pixelsPerUnit.y;
		if (parent != nullptr)
		{
			localPosition.x = position.x - parent->position.x;
			localPosition.y = position.y - parent->position.y;
		}
		else
			localPosition = position;
		UpdateGlobalPosition();
		OnTransform();
	}

	void Element::SetPosition(const Vector2& pos)
	{
		pixelPosition.x = (int)(pos.x * pixelsPerUnit.x);
		pixelPosition.y = (int)(pos.y * pixelsPerUnit.y);
		if (parent != nullptr)
		{
			localPosition.x = position.x - parent->position.x;
			localPosition.y = position.y - parent->position.y;
		}
		else
			localPosition = position;
		UpdateGlobalPosition();
		OnTransform();
	}

	void Element::SetLocalPosition(const Vector2& pos)
	{
		localPosition = pos;
		UpdateGlobalPosition();
		pixelPosition.x = (int)(position.x * pixelsPerUnit.x);
		pixelPosition.y = (int)(position.y * pixelsPerUnit.y);
		OnTransform();
	}

	void Element::SetGlobalScale(const Vector2& scale)
	{
		globalScale = scale;
		if (parent != nullptr)
		{
			localScale.x = scale.x / parent->globalScale.x;
			localScale.y = scale.y / parent->globalScale.y;
		}
		else
			localScale = scale;
		OnTransform();
	}

	void Element::SetLocalScale(const Vector2& scale)
	{
		localScale = scale;
		UpdateGlobalScale();
		OnTransform();
	}

	void Element::DrawElements()
	{
		for (Element* element : elements)
			element->Draw();
	}

	void Element::UpdateGlobalPosition()
	{
		if (parent != nullptr)
			position = localPosition + parent->position;
		else
			position = localPosition;
		for (Element* element : children)
			element->UpdateGlobalPosition();
	}

	void Element::UpdateGlobalScale()
	{
		if (parent != nullptr)
		{
			globalScale.x = localScale.x * parent->globalScale.x;
			globalScale.y = localScale.y * parent->globalScale.y;
		}
		else
			globalScale = localScale;
		for (Element* element : children)
			element->UpdateGlobalScale();
	}

	std::vector<Element*> Element::elements = std::vector<Element*>();
}