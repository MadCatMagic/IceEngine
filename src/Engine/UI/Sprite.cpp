#include "UI.h"

namespace UI
{
	Sprite::Sprite() { }

	Sprite::Sprite(const std::string& filepath)
	{
		this->filepath = filepath;
	}

	Sprite::~Sprite()
	{
		if (texture != nullptr)
			delete texture;
	}

	void Sprite::SetPath(const std::string& filepath)
	{
		this->filepath = filepath;
	}

	void Sprite::LoadTexture()
	{
		texture = new Texture2D(filepath);
	}

	void Sprite::Bind() const
	{
		texture->Bind();
	}

	void Sprite::Unbind() const
	{
		texture->Unbind();
	}
}