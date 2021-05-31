#include "Engine/UI/Panel.h"

UI::Panel::Panel(const Vector2i& size, const Vector2i& pixelPos)
{
	this->size = size;
	this->SetPixelPosition(pixelPos);
}

UI::Panel::Panel(const Vector2i& size, const Vector2i& pixelPos, Sprite* sprite)
{
	this->size = size;
	this->SetPixelPosition(pixelPos);
	CreateRenderer(sprite);
}

UI::Panel::~Panel()
{
	if (spriteRenderer != nullptr)
		delete spriteRenderer;
}

void UI::Panel::Draw()
{
	spriteRenderer->Draw(position, Vector2(size.x / pixelsPerUnit.x, size.y / pixelsPerUnit.y));
}

void UI::Panel::OnTransform() { }

void UI::Panel::HandleMouseClick(int action, Vector2i location) { }

void UI::Panel::SetSprite(Sprite* sprite)
{
	CreateRenderer(sprite);
}

void UI::Panel::CreateRenderer(Sprite* sprite)
{
	spriteRenderer = new SpriteRenderer(sprite);
}