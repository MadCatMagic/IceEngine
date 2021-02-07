#pragma once
#include "Matrix.h"
#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(float zNear, float zFar, float fov);
	~Camera();

	float zNear{};
	float zFar{};
	float fov{};

	Matrix4x4 GetProjectionMatrix(const Transform& transform);
	Matrix4x4 GetViewMatrix(const Transform& transform);
};

