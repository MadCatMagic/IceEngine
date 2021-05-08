#pragma once
#include "Texture/Texture2D.h"

class RenderTexture : public Texture
{
public:

	bool renderTextureConstructed{};

	// both depth and colour
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