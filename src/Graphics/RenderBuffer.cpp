#include "RenderBuffer.h"

RenderBuffer::RenderBuffer()
{
	type = RenderBufferType::NONE;
	id = -1;
	size = Vector2i::zero;
}

RenderBuffer::RenderBuffer(RenderBufferType type, int sizeX, int sizeY)
{
	this->type = type;
	this->size = Vector2i(sizeX, sizeY);
	ConstructBuffer();
}

RenderBuffer::RenderBuffer(RenderBufferType type, Vector2i size)
{
	this->type = type;
	this->size = size;
	ConstructBuffer();
}

RenderBuffer::RenderBuffer(const RenderBuffer& obj)
{
	this->id = obj.id;
	this->type = obj.type;
	this->size = obj.size;
	this->bufferConstructed = obj.bufferConstructed;
}

RenderBuffer::RenderBuffer(RenderBuffer&& obj) noexcept
{
	this->id = obj.id;
	obj.id = -1;
	this->type = obj.type;
	obj.type = RenderBufferType::NONE;
	this->size = obj.size;
	obj.size = Vector2i::zero;
	this->bufferConstructed = obj.bufferConstructed;
	obj.bufferConstructed = false;
}

RenderBuffer::~RenderBuffer()
{
	if (bufferConstructed)
		glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::SetParams(RenderBufferType type, int sizeX, int sizeY)
{
	this->type = type;
	this->size = Vector2i(sizeX, sizeY);
	ConstructBuffer();
}

void RenderBuffer::SetParams(RenderBufferType type, Vector2i size)
{
	this->type = type;
	this->size = size;
	ConstructBuffer();
}

RenderBuffer& RenderBuffer::operator=(const RenderBuffer& other)
{
	this->id = other.id;
	this->type = other.type;
	this->size = other.size;
	this->bufferConstructed = other.bufferConstructed;
	return *this;
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept
{
	this->id = other.id;
	other.id = -1;
	this->type = other.type;
	other.type = RenderBufferType::NONE;
	this->size = other.size;
	other.size = Vector2i::zero;
	this->bufferConstructed = other.bufferConstructed;
	other.bufferConstructed = false;
	return *this;
}

void RenderBuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::ConstructBuffer()
{
	glGenRenderbuffers(1, &id);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)type, size.x, size.y);
	bufferConstructed = true;
}
