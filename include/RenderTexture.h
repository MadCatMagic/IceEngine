#pragma once
#include "Renderer.h"

#include "RenderBuffer.h"
#include "Texture2D.h"

class RenderTexture
{
public:
	enum class AttachmentType {
		COLOR0 = GL_COLOR_ATTACHMENT0,
		DEPTH = GL_DEPTH_ATTACHMENT
	};

	bool renderTextureConstructed{};

	RenderTexture();
	//RenderTexture(const RenderTexture& obj);
	//RenderTexture(RenderTexture&& obj) noexcept;
	~RenderTexture();

	void SetAttachment(AttachmentType type, const RenderBuffer& obj);
	void SetAttachment(AttachmentType type, const Texture2D& obj, int lod);

	void SetDrawBuffers(unsigned int size, const AttachmentType* buffers) const;

	bool TextureOK() const;

	//RenderTexture& operator=(const RenderTexture& other);
	//RenderTexture& operator=(RenderTexture&& other) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	unsigned int id;

	void ConstructBuffer();
};