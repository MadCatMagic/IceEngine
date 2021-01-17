#include "Texture2D.h"

Texture2D::Texture2D()
{
	id = -1;
}

Texture2D::Texture2D(unsigned int lod, TextureFormat internalFormat, int sizeX, int sizeY, TextureFormat format)
{
	SetFormat(lod, internalFormat, sizeX, sizeY, format);
}

Texture2D::Texture2D(unsigned int lod, TextureFormat internalFormat, const Vector2i& size, TextureFormat format)
{
	SetFormat(lod, internalFormat, size, format);
}

Texture2D::Texture2D(const Texture2D& obj)
{
	this->id = obj.id;
	this->formatted = obj.formatted;
}

Texture2D::Texture2D(Texture2D&& obj) noexcept
{
	this->id = obj.id;
	obj.id = -1;
	this->formatted = obj.formatted;
	obj.formatted = false;
}

Texture2D::~Texture2D()
{
	if (formatted)
		glDeleteTextures(1, &id);
}

void Texture2D::SetFormat(unsigned int lod, TextureFormat internalFormat, int sizeX, int sizeY, TextureFormat format)
{
	CreateTexture();
	glTexImage2D(GL_TEXTURE_2D, lod, (int)internalFormat, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

void Texture2D::SetFormat(unsigned int lod, TextureFormat internalFormat, const Vector2i& size, TextureFormat format)
{
	CreateTexture();
	glTexImage2D(GL_TEXTURE_2D, lod, (int)internalFormat, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

void Texture2D::SetFiltering(GLenum filter, GLenum type) const
{
	glTexParameteri(GL_TEXTURE_2D, filter, type);
}

void Texture2D::SetDefaultFiltering() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
	this->id = other.id;
	this->formatted = other.formatted;
	return *this;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
	this->id = other.id;
	other.id = -1;
	this->formatted = other.formatted;
	other.formatted = false;
	return *this;
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
	formatted = true;
}
