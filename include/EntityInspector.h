#pragma once
#include "Engine/Entity.h"
#include "Engine/UI.h"

class EntityInspector : public Behaviour
{
public:
	EntityInspector();
	~EntityInspector();

	void SelectEntity(Entity* target);

	void SetOffset(int offset);

private:
	Entity* target{ nullptr };
	UI::Text* text{ nullptr };

	int offset{ 0 };

	// Inherited via Behaviour
	void Start();
	void Update();
};

