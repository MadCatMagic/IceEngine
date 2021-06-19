#include "Engine/Graphics/PostProcessEffect.h"
#include "Engine/Entity.h"
#include "Engine/Graphics/Camera.h"

PostProcessEffect::PostProcessEffect() { }

PostProcessEffect::~PostProcessEffect() { }

void PostProcessEffect::Start()
{
	camera = entity->GetBehaviour<Camera>();
}