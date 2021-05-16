#include "Light.h"

Light::Light()
{
	lights.push_back(this);
}

Light::~Light()
{
	std::vector<Light*>::iterator index = std::find(lights.begin(), lights.end(), this);
	lights.erase(index);
}

Matrix4x4 Light::GetMatrix()
{
	Matrix4x4 ortho = Matrix4x4::OrthoMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 10.0f, -10.0f);
	Matrix4x4 view = Matrix4x4::ViewMatrix(transform->GetPos(), -transform->Up(), Vector3(0.0f, 1.0f, 0.0f));
	return ortho * view;
}

std::vector<Light*> Light::lights = std::vector<Light*>();