#include "UI.h"

namespace UI
{
	void Button::HandleMouseClick(int button, Vector2i location)
	{
		// if in bounds do stuff
		if (pixelPosition.x <= location.x && pixelPosition.x + size.x >= location.x && 
			pixelPosition.y <= location.y && pixelPosition.y + size.y >= location.y)
		{
			action(button);
		}
	}

	void Button::Draw() const
	{
	}

	void Button::SetAction(std::function<void(int)> action)
	{
		this->action = action;
	}
}
