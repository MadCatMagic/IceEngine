#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
public:
	Texture2D(const std::string& filepath);
	Texture2D(Format format, const Vector2i& size, unsigned int lod);
	Texture2D(Format format, const Vector2i& size, WrapMode wrapMode, unsigned int lod);
	Texture2D(Format format, const Vector2i& size, unsigned int lod, const void* data);
	Texture2D(Format format, const Vector2i& size, WrapMode wrapMode, unsigned int lod, const void* data);
	Texture2D(const Texture2D& obj);
	Texture2D(Texture2D&& obj) noexcept;
	~Texture2D();

	Texture2D& operator=(const Texture2D& other);
	Texture2D& operator=(Texture2D&& other) noexcept;

	void ApplyFiltering() const override;
	void Bind() const override;
	void Unbind() const override;
	void SetBorderColour(const Vector4& colour) const override;

	void CreateTexture();
	void CreateTexture(const void* data);

	inline unsigned int GetID() const { return id; }
private:
	unsigned int id;
};