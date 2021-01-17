#pragma once
#include "Renderer.h"
#include "Vector.h"

class Texture2D
{
public:
	enum class TextureFormat {
		RGB = GL_RGB, RGBA = GL_RGBA
	};

	Texture2D();
	Texture2D(unsigned int lod, TextureFormat internalFormat, int sizeX, int sizeY, TextureFormat format);
	Texture2D(unsigned int lod, TextureFormat internalFormat, const Vector2i& size, TextureFormat format);
	Texture2D(const Texture2D& obj);
	Texture2D(Texture2D&& obj) noexcept;
	~Texture2D();

	void SetFormat(unsigned int lod, TextureFormat internalFormat, int sizeX, int sizeY, TextureFormat format);
	void SetFormat(unsigned int lod, TextureFormat internalFormat, const Vector2i& size, TextureFormat format);
	void SetFiltering(GLenum filter, GLenum type) const;
	void SetDefaultFiltering() const;

	Texture2D& operator=(const Texture2D& other);
	Texture2D& operator=(Texture2D&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() const { return id; }
private:
	bool formatted{};
	unsigned int id;

	void CreateTexture();
};

