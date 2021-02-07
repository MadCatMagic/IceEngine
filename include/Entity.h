#pragma once

#include "Behaviour.h"
#include <vector>
#include <stdexcept>
#include <iostream>

class Entity
{
public:
	Transform* transform{ nullptr };

	Entity();
	~Entity();

	void DefaultTransform();
	void AssignTransform(Transform* transform);

	// way to assign behaviours
	template<typename T>
	T* AddBehaviour();

private:
	std::vector<Behaviour*> behaviours;
};

template<typename T>
T* Entity::AddBehaviour()
{
	T* behaviour = new T();
	behaviour->entity = this;
	behaviour->transform = transform;
	behaviours.push_back(behaviour);
	return behaviour;
}
