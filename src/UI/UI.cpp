#include "UI.h"

namespace UI
{
	extern void Initialize(Vector2i windowSize)
	{
		pixelsPerUnit.x = windowSize.x / 2.0f;
		pixelsPerUnit.y = windowSize.y / 2.0f;
	}
}