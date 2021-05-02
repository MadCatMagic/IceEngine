#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera() { }

Camera::Camera(float zNear, float zFar, float fov, float widthHeightRatio)
{
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = fov;
	this->widthHeightRatio = widthHeightRatio;
}

Camera::~Camera() { }

// base projection matrix
// from world to screen space
Matrix4x4 Camera::GetProjectionMatrix()
{
	const float f = 1.0f / tan(fov * 0.5f);
	const float q = zFar / (zFar - zNear);
	Matrix4x4 matrix = Matrix4x4(
		Vector4(f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, f * widthHeightRatio, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, q, 1.0f),
		Vector4(0.0f, 0.0f, -zNear * q, 0.0f)
	);
	// multiply by view matrix
	matrix = matrix * GetViewMatrix();
	return matrix;
}

// position translation matrix
// from world to view space
Matrix4x4 Camera::GetViewMatrix()
{
	// you have to use the conjugate for correct camera movement
	// I DONT KNOW WHY WTF
	return Matrix4x4::LookAt(
		transform->GetPos(), 
		transform->GetPos() - transform->Forward(),
		Vector3(0.0f, 1.0f, 0.0f)
	);
}
