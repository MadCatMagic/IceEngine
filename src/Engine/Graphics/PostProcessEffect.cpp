#include "Engine/PostProcessEffect.h"
#include "Engine/Entity.h"
#include "Engine/Camera.h"

PostProcessEffect::PostProcessEffect() { }

PostProcessEffect::~PostProcessEffect() { }

void PostProcessEffect::Start()
{
	camera = entity->GetBehaviour<Camera>();
}