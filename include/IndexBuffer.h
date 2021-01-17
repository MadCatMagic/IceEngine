#pragma once

class IndexBuffer
{
private:
	unsigned int id;
	unsigned int count;
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const IndexBuffer& obj);
	IndexBuffer(IndexBuffer&& obj) noexcept;
	~IndexBuffer();

	IndexBuffer& operator=(const IndexBuffer& other);
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};

