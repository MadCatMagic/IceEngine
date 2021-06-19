#pragma once
#include "Engine/Graphics/PostProcessEffect.h"

class TemporalReprojection : public PostProcessEffect
{
public:
	TemporalReprojection();
	~TemporalReprojection();

	virtual void OnPostProcess(RenderTexture* src, RenderTexture* dest) override;

private:
	void Awake();

	RenderTexture* previousFrame{ nullptr };
	Matrix4x4 previousCamMatrix;

	class Shader* reprojectionShader{ nullptr };
	class Material* reprojectionMaterial{ nullptr };

	bool notFirstFrame{ false };
};

