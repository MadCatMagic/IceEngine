#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
	delete transform;
	for (auto behaviour : behaviours)
		delete behaviour;
}

// default
void Entity::DefaultTransform()
{
	this->transform = new Transform();
	this->transform->entity = this;
}

// should be a heap allocated object
void Entity::AssignTransform(Transform* transform)
{
	this->transform = transform;
	transform->entity = this;
}