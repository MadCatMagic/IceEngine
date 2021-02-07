#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera() { }

Camera::Camera(float zNear, float zFar, float fov)
{
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = fov;
}

Camera::~Camera()
{

}

// base projection matrix
// from world to screen space
Matrix4x4 Camera::GetProjectionMatrix(const Transform& transform)
{
	float f = 1.0f / tan(fov / 2.0f);
	float q = zFar / (zFar - zNear);
	Matrix4x4 matrix = Matrix4x4(
		Vector4(   f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f,    f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f,    q, 1.0f),
		Vector4(0.0f, 0.0f, -zNear * q, 0.0f)
	);
	// multiply by view matrix
	matrix = matrix * GetViewMatrix(transform);
	return matrix;
}

// position translation matrix
// from world to view space
Matrix4x4 Camera::GetViewMatrix(const Transform& transform)
{
	// you have to use the conjugate for correct camera movement
	// I DONT KNOW WHY WTF
	return Matrix4x4::LookAt(
		transform.position, 
		transform.position + transform.rotation.RotateVector(Vector3(0.0f, 0.0f, -1.0f)), 
		Vector3(0.0f, 1.0f, 0.0f)
	);
}
