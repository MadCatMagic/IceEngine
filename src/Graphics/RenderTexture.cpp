#include "RenderTexture.h"
#include <iostream>

RenderTexture::RenderTexture()
{
	ConstructBuffer();
}
/*
RenderTexture::RenderTexture(const RenderTexture& obj)
{
	this->id = obj.id;
	this->renderTextureConstructed = obj.renderTextureConstructed;
}

RenderTexture::RenderTexture(RenderTexture&& obj) noexcept
{
	this->id = obj.id;
	obj.id = -1;
	this->renderTextureConstructed = obj.renderTextureConstructed;
	obj.renderTextureConstructed = false;
}*/

RenderTexture::~RenderTexture()
{
	if (renderTextureConstructed)
		glDeleteFramebuffers(1, &id);
}

void RenderTexture::SetAttachment(AttachmentType type, const RenderBuffer& obj)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, (int)type, GL_RENDERBUFFER, obj.GetID());
}

void RenderTexture::SetAttachment(AttachmentType type, const Texture2D& obj, int lod)
{
	glFramebufferTexture(GL_FRAMEBUFFER, (int)type, obj.GetID(), lod);
}

void RenderTexture::SetDrawBuffers(unsigned int size, const AttachmentType* buffers) const
{
	glDrawBuffers(size, (GLenum*)buffers);
}

bool RenderTexture::TextureOK() const
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}
/*
RenderTexture& RenderTexture::operator=(const RenderTexture& other)
{
	this->id = other.id;
	this->renderTextureConstructed = other.renderTextureConstructed;
	return *this;
}

RenderTexture& RenderTexture::operator=(RenderTexture&& other) noexcept
{
	this->id = other.id;
	other.id = -1;
	this->renderTextureConstructed = other.renderTextureConstructed;
	other.renderTextureConstructed = false;
	return *this;
}*/

void RenderTexture::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void RenderTexture::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::ConstructBuffer()
{
	glGenFramebuffers(1, &id);
	Bind();
	renderTextureConstructed = true;
}
