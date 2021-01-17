#pragma once
#include <vector>
#include "Transform.h"

class Entity;

class Behaviour
{
public:
	Entity* entity{ nullptr };
	Transform* transform{ nullptr };

	Behaviour();
	Behaviour(const Behaviour& obj);
	Behaviour(Behaviour&& obj) noexcept;
	~Behaviour();

	Behaviour& operator=(const Behaviour& other);
	Behaviour& operator=(Behaviour&& other) noexcept;

	virtual void Update();

	static void UpdateBehaviours();
	static void AddBehaviour(Behaviour* behaviour);

private:
	static std::vector<Behaviour*> behaviourList;
};

