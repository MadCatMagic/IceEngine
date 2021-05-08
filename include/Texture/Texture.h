#pragma once
#include <GL/glew.h>
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
	Format formatType{ Format::RGBA };
	WrapMode wrapMode{ WrapMode::Repeat };

	const int dimension;
	int width{ 0 };
	int height{ 0 };

	unsigned int lod{ 0 };

	inline virtual void ApplyFiltering() const {};
	inline virtual void Bind() const {};
	inline virtual void Unbind() const {};

protected:
	static Format FormatType(Format format);
};