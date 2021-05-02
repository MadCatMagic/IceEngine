#pragma once
#include "Entity.h"
#include "UI.h"

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
	virtual void Start() override;
	virtual void Update() override;
};

