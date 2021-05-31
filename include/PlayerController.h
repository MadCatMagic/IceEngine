#pragma once

#include "Engine/Behaviour.h"
#include "Engine/Camera.h"

class PlayerController : public Behaviour
{
private:
	void Update();

	const float speed = 0.04f;
};

