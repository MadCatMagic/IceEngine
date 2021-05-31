#include "Engine/Camera.h"

#include <cmath>
#include <iostream>

Camera::Camera() { }

void Camera::SetParams(float zNear, float zFar, float fov, float widthHeightRatio)
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
	return Matrix4x4::ProjectionMatrix(fov, zNear, zFar, widthHeightRatio) * GetViewMatrix();
}

// position translation matrix
// from world to view space
Matrix4x4 Camera::GetViewMatrix()
{
	return Matrix4x4::ViewMatrix(transform->GetPos(), transform->Forward(), Vector3(0.0f, 1.0f, 0.0f));
}
