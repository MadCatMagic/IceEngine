#pragma once
#include "Behaviour.h"
#include "Texture/RenderTexture.h"

class PostProcessEffect : public Behaviour
{
public:
	PostProcessEffect();
	~PostProcessEffect();

	virtual void OnPostProcess(RenderTexture* src, RenderTexture* dest) = 0;

	class Camera* camera{ nullptr };

private:
	void Start();
};

