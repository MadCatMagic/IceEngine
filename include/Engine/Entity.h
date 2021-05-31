#pragma once

#include "Behaviour.h"
#include <vector>

class Entity
{
public:
	Transform* transform{ nullptr };

	Entity();
	~Entity();

	void AssignTransform(Transform* transform);

	// way to assign behaviours
	// never add two of the same behaviour to an object
	// for many reasons, such as GetBehaviour only returning the first valid behaviour
	template<typename T>
	T* AddBehaviour();

	// get behaviour
	// returns behaviour of type T from behaviourList
	// if no behaviour of type T exists returns nullptr
	template<typename T>
	T* GetBehaviour();

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

template<typename T>
T* Entity::GetBehaviour()
{
	for (Behaviour* b: behaviours)
	{
		// weird casting to get derived from the Behaviour base class
		T* t = dynamic_cast<T*>(b);
		// if it doesnt work dynamic_cast returns nullptr
		if (t != nullptr)
			return t;
	}

	// exception case: if no behaviour of type T exists return null
	return nullptr;
}