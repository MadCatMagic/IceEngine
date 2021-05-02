#pragma once
#include "Matrix.h"
#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(float zNear, float zFar, float fov, float widthHeightRatio);
	~Camera();

	float zNear{};
	float zFar{};
	float fov{};
	float widthHeightRatio{};

	Transform* transform{ nullptr };

	Matrix4x4 GetProjectionMatrix();
	Matrix4x4 GetViewMatrix();
};

