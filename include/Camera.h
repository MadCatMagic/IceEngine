#pragma once
#include "Matrix.h"
#include "Behaviour.h"

class Camera
{
public:
	Camera();
	Camera(const Transform& transform);
	Camera(const Transform& transform, float zNear, float zFar, float fov);
	~Camera();

	Transform transform;
	Vector3 viewDir;

	float zNear{};
	float zFar{};
	float fov{};

	Matrix4x4 GetProjectionMatrix();
	Matrix4x4 GetViewMatrix();
};

