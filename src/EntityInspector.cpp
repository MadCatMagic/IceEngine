#include "EntityInspector.h"
#include <iostream>
#include "..\include\EntityInspector.h"

EntityInspector::EntityInspector() { }

EntityInspector::~EntityInspector()
{
	if (text != nullptr)
		delete text;
}

void EntityInspector::SelectEntity(Entity* target)
{
	this->target = target;
}

void EntityInspector::SetOffset(int offset)
{
	this->offset = offset;
}

void EntityInspector::Start()
{
	// text testing
	text = new UI::Text(15, Vector2i(-620, 400 - offset), "placeholder", "res/fonts/Helvetica.ttf");
	text->CreateRenderer();
}

void EntityInspector::Update()
{
	if (target != nullptr)
	{
		text->SetText(
			"Inspector\nPosition: " + target->transform->GetPos().ToString() + "\n" +
			"Rotation: " + target->transform->GetRot().ToEuler().ToString() + "\n" +
			"Scale: " + target->transform->GetGlobalScale().ToString()
		);
	}
}
