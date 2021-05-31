#pragma once
#include "Matrix.h"
#include "Behaviour.h"
#include "Texture/RenderTexture.h"

class Behaviour;
class Camera : public Behaviour
{
public:
	Camera();
	~Camera();

	void SetParams(float zNear, float zFar, float fov, float widthHeightRatio);

	float zNear{};
	float zFar{};
	float fov{};
	float widthHeightRatio{};

	RenderTexture* target{ nullptr };

	Matrix4x4 GetProjectionMatrix();
	Matrix4x4 GetViewMatrix();
};

