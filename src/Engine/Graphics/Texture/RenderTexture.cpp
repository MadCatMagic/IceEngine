#include "Texture/RenderTexture.h"
#include <iostream>

RenderTexture::RenderTexture(int width, int height, Format depth, Format colour)
	: Texture(Dimension::Two)
{
	this->id = -1;
	this->width = width;
	this->height = height;
	this->depthFormat = depth;
	this->colourFormat = colour;
}

RenderTexture::~RenderTexture()
{
	if (generatedBuffers)
	{
		glDeleteFramebuffers(1, &id);
		if (colourFormat != Format::None)
			delete colourBuffer;
		if (depthFormat != Format::None)
			delete depthBuffer;
	}
}

// creates rendertexture and tex2d buffers
// DONT TRY TO USE RENDERTEXTURE BEFORE CALLING THIS
void RenderTexture::GenerateBuffers()
{
	// generate and bind framebuffer (render texture)
	glGenFramebuffers(1, &id);
	Bind();

	if (colourFormat != Format::None)
	{
		// create colourBuffer and assign to framebuffer
		colourBuffer = new Texture2D(colourFormat, Vector2i(width, height), 0);
		colourBuffer->CreateTexture();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBuffer->GetID(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}
	else
		glDrawBuffer(GL_NONE);

	if (depthFormat != Format::None)
	{
		// create depthBuffer amd assign to framebuffer
		depthBuffer = new Texture2D(depthFormat, Vector2i(width, height), 0);
		depthBuffer->CreateTexture();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuffer->GetID(), 0);
	}

	generatedBuffers = true;
	if (!TextureOK())
		std::cout << "Rendertexture error!" << std::endl;
	Unbind();
}

// makes sure nothing is broken
bool RenderTexture::TextureOK() const
{
	Bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}

void RenderTexture::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void RenderTexture::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}