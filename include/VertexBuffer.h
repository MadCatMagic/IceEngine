#pragma once

class VertexBuffer
{
private:
	unsigned int id;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& obj);
	VertexBuffer(VertexBuffer&& obj) noexcept;
	~VertexBuffer();
	
	VertexBuffer& operator=(const VertexBuffer& other);
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void SetData(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
};

