#include "PlayerController.h"
#include "Input.h"
#include <iostream>

void PlayerController::Update()
{
    // camera directions
    Vector3 forward = -transform->Forward();
    Vector3 right = transform->Right();

    // movement
    if (Input::spaceState == 1 || Input::spaceState == 2)
        transform->Move(Vector3(0.0f, speed, 0.0f));
    else if (Input::lshiftState == 1 || Input::lshiftState == 2)
        transform->Move(Vector3(0.0f, -speed, 0.0f));
    if (Input::wState == 1 || Input::wState == 2)
        transform->Move(forward * speed);
    else if (Input::sState == 1 || Input::sState == 2)
        transform->Move(-forward * speed);
    if (Input::dState == 1 || Input::dState == 2)
        transform->Move(-right * speed);
    else if (Input::aState == 1 || Input::aState == 2)
        transform->Move(right * speed);
}

void PlayerController::SetCam(Camera* camera)
{
	cam = camera;
}

Matrix4x4 PlayerController::GetCamMatrix() const
{
	return cam->GetProjectionMatrix(*transform->child);
}
