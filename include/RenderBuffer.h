#pragma once

#include <GL/glew.h>
#include "Vector.h"

class RenderBuffer
{
public:
	enum class RenderBufferType {
		NONE = -1,
		RGBA8 = GL_RGBA8,
		DEPTH = GL_DEPTH_COMPONENT
	};

	bool bufferConstructed{};

	RenderBuffer(); // creates empty buffer
	RenderBuffer(RenderBufferType type, int sizeX, int sizeY); // creates buffer
	RenderBuffer(RenderBufferType type, Vector2i size); // creates buffer
	RenderBuffer(const RenderBuffer& obj);
	RenderBuffer(RenderBuffer&& obj) noexcept;
	~RenderBuffer();

	void SetParams(RenderBufferType type, int sizeX, int sizeY);
	void SetParams(RenderBufferType type, Vector2i size);

	RenderBuffer& operator=(const RenderBuffer& other);
	RenderBuffer& operator=(RenderBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() const { return id; }
private:
	RenderBufferType type;
	Vector2i size;
	unsigned int id;

	void ConstructBuffer();
};