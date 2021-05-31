#include "Engine/UI/Text.h"

namespace UI
{
    Text::Text(unsigned int size, const Vector2i& pixelPos)
    {
        this->size = size;
        this->text = "missing text";
    }

    Text::Text(unsigned int size, const Vector2i& pixelPos, const std::string& text)
    {
        this->size = size;
        SetPixelPosition(pixelPos);
        this->text = text;
    }

    Text::Text(unsigned int size, const Vector2i& pixelPos, const std::string& text, const std::string& filepath)
    {
        this->size = size;
        SetPixelPosition(pixelPos);
        this->text = text;
        this->font = filepath;
    }

    Text::~Text()
    {
        delete textRenderer;
    }

    void Text::Draw()
    {
        if (textRenderer != nullptr)
            textRenderer->Draw(position, text, 1.0f, Vector3::one);
    }

    void Text::OnTransform() { }

    void Text::SetText(const std::string& text)
    {
        this->text = text;
    }

    void Text::SetFont(const std::string& filepath)
    {
        font = filepath;
    }

    void Text::CreateRenderer()
    {
        textRenderer = new TextRenderer(font, size);
        textRenderer->SetupRenderer();
        textRenderer->LoadChars();
    }
}