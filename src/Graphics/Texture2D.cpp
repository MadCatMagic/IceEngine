#include "Texture.h"

Texture2D::Texture2D(const std::string& filepath)
	: Texture(2)
{
	id = -1;
}

Texture2D::Texture2D(Format format, const Vector2i& size, unsigned int lod)
	: Texture(2)
{
	id = -1;
	this->format = format;
	this->width = size.x;
	this->height = size.y;
	this->lod = lod;
	CreateTexture();
}

Texture2D::Texture2D(Format format, const Vector2i& size, WrapMode wrapMode, unsigned int lod)
	: Texture(2)
{
	id = -1;
	this->format = format;
	this->width = size.x;
	this->height = size.y;
	this->lod = lod;
	this->wrapMode = wrapMode;
	CreateTexture();
	ApplyFiltering();
}

Texture2D::Texture2D(const Texture2D& obj)
	: Texture(2)
{
	this->id = obj.id;
	this->format = obj.format;
	this->wrapMode = obj.wrapMode;
	this->width = obj.width;
	this->height = obj.height;
	this->lod = obj.lod;
}

Texture2D::Texture2D(Texture2D&& obj) noexcept
	: Texture(2)
{
	this->id = obj.id;
	obj.id = -1;
	this->format = obj.format;
	obj.format = Format::RGBA;
	this->wrapMode = obj.wrapMode;
	obj.wrapMode = WrapMode::Repeat;
	this->width = obj.width;
	obj.width = 0;
	this->height = obj.height;
	obj.height = 0;
	this->lod = obj.lod;
	obj.lod = 0;
}

Texture2D::~Texture2D()
{
	if (id != -1)
		glDeleteTextures(1, &id);
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
	this->id = other.id;
	this->format = other.format;
	this->wrapMode = other.wrapMode;
	this->width = other.width;
	this->height = other.height;
	this->lod = other.lod;
	return *this;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
	this->id = other.id;
	other.id = -1;
	this->format = other.format;
	other.format = Format::RGBA;
	this->wrapMode = other.wrapMode;
	other.wrapMode = WrapMode::Repeat;
	this->width = other.width;
	other.width = 0;
	this->height = other.height;
	other.height = 0;
	this->lod = other.lod;
	other.lod = 0;
	return *this;
}

// make sure texture is bound AND created
void Texture2D::ApplyFiltering() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrapMode);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::CreateTexture()
{
	glGenTextures(1, &id);
	Bind();
	glTexImage2D(GL_TEXTURE_2D, lod, (int)format, width, height, 0, (int)format, GL_UNSIGNED_BYTE, 0);
}
