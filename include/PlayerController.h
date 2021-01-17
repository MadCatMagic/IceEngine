#pragma once

#include "Behaviour.h"

class PlayerController : public Behaviour
{
public:
	PlayerController();
	void Update() override;
};

