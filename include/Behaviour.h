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

	// called before the first frame
	inline virtual void Start() {};
	// called every frame
	inline virtual void Update() {};

	static void StartBehaviours();
	static void UpdateBehaviours();
	static void ReleaseBehaviours();
private:
	static std::vector<Behaviour*> behaviourList;
};

