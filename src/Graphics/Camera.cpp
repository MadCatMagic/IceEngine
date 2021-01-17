#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera() { }

Camera::Camera(const Transform& transform)
{
	this->transform = transform;
}

Camera::Camera(const Transform& transform, float zNear, float zFar, float fov)
{
	this->transform = transform;
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = fov;
}

Camera::~Camera()
{

}

Matrix4x4 Camera::GetProjectionMatrix()
{
	// base projection matrix
	// from world to screen space
	float f = 1.0f / tan(fov / 2.0f);
	float q = zFar / (zFar - zNear);
	Matrix4x4 matrix = Matrix4x4(
		Vector4(   f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f,    f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f,    q, 1.0f),
		Vector4(0.0f, 0.0f, -zNear * q, 0.0f)
	);
	// multiply by view matrix
	matrix = matrix * GetViewMatrix();
	return matrix;
}

Matrix4x4 Camera::GetViewMatrix()
{
	// position translation matrix
	// from world to view space
	return Matrix4x4::LookAt(transform.position, transform.position + viewDir, Vector3(0.0f, 1.0f, 0.0f));
}
