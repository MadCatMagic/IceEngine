#pragma once

#include "Behaviour.h"
#include "Camera.h"

class PlayerController : public Behaviour
{
public:
	void Update() override;

	void SetCam(Camera* camera);

	Matrix4x4 GetCamMatrix() const;

	const float speed = 0.04f;
private:
	Camera* cam{ nullptr };
};

