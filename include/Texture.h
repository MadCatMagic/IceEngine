#pragma once

#include "Renderer.h"
#include "Vector.h"

class Texture
{
public:
	Texture(const int dim) : dimension(dim) {}

	enum class Format {
		None = -1,
		RGB = GL_RGB, RGB8 = GL_RGB8, RGBFloat = GL_RGB32F,
		RGBA = GL_RGBA, RGBA8 = GL_RGBA8, RGBAFloat = GL_RGBA32F,
		Red = GL_RED,
		Depth = GL_DEPTH_COMPONENT, Depth16 = GL_DEPTH_COMPONENT16, Depth24 = GL_DEPTH_COMPONENT24
	};

	enum class WrapMode {
		Repeat = GL_REPEAT, Clamp = GL_CLAMP_TO_EDGE, Mirror = GL_MIRRORED_REPEAT
	};

	Format format{ Format::RGBA };
	WrapMode wrapMode{ WrapMode::Repeat };

	const int dimension;
	int width{ 0 };
	int height{ 0 };

	unsigned int lod{ 0 };

	inline virtual void ApplyFiltering() const {};
	inline virtual void Bind() const {};
	inline virtual void Unbind() const {};
};

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

	void CreateTexture();
	void CreateTexture(const void* data);

	inline unsigned int GetID() const { return id; }
private:
	unsigned int id;
};

class RenderTexture : public Texture
{
public:

	bool renderTextureConstructed{};

	RenderTexture(int width, int height, Format depth, Format colour);
	//RenderTexture(const RenderTexture& obj);
	//RenderTexture(RenderTexture&& obj) noexcept;
	~RenderTexture();

	void GenerateBuffers();

	bool TextureOK() const;

	Texture2D* colourBuffer{ nullptr };
	Texture2D* depthBuffer{ nullptr };

	//RenderTexture& operator=(const RenderTexture& other);
	//RenderTexture& operator=(RenderTexture&& other) noexcept;

	void Bind() const override;
	void Unbind() const override;

	inline unsigned int GetID() { return id; };
private:
	Format depthFormat;
	Format colourFormat;

	unsigned int id;
	bool generatedBuffers{ false };
};