#include "Entity.h"

Entity::Entity()
{
	this->transform = new Transform();
	this->transform->entity = this;
}

Entity::~Entity()
{
	delete transform;
	for (auto behaviour : behaviours)
		delete behaviour;
}

// should be a heap allocated object
void Entity::AssignTransform(Transform* transform)
{
	this->transform = transform;
	transform->entity = this;
}