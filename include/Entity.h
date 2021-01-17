#pragma once

#include "Behaviour.h"
#include <vector>

class Entity
{
public:
	Transform* transform{ nullptr };

	Entity();
	~Entity();

	void DefaultTransform();
	void AssignTransform(Transform* transform);

	template<typename T>
	void AddBehaviour();

private:
	std::vector<Behaviour*> behaviours;
};

template<typename T>
void Entity::AddBehaviour()
{
	T* behaviour = new T();
	behaviour->entity = this;
	behaviour->transform = transform;
	behaviours.push_back(behaviour);
}