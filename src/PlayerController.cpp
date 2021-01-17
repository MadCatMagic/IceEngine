#include "PlayerController.h"
#include <iostream>

PlayerController::PlayerController()
{
	Behaviour::AddBehaviour(this);
}

void PlayerController::Update()
{
	std::cout << "update" << std::endl;
}
