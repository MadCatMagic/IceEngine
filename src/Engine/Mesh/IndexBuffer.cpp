#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer()
{
	id = -1;
	count = -1;
}

IndexBuffer::IndexBuffer(const IndexBuffer& obj) {
	// Copy Constructor
	// copy of object is created
	this->id = obj.id;
	this->count = obj.count;
	// Deep copying
}

IndexBuffer::IndexBuffer(IndexBuffer&& obj) noexcept {
	// Move constructor
	// It will simply shift the resources,
	// without creating a copy.
	this->id = obj.id;
	obj.id = -1;
	this->count = obj.count;
	obj.count = 0;
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: count(count)
{
	glGenBuffers(1, &id);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &id);
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other)
{
	this->id = other.id;
	this->count = other.count;
	return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	this->id = other.id;
	other.id = -1;
	this->count = other.count;
	other.count = 0;
	return *this;
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
