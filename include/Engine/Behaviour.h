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

	static void AwakeBehaviours();
	static void StartBehaviours();
	static void UpdateBehaviours();
	static void ReleaseBehaviours();

	static void DrawGizmosFromBehaviours();

private:
	// called before the first frame
	inline virtual void Start() {};
	// called every frame
	inline virtual void Update() {};
	// called before start()
	inline virtual void Awake() {};

	// on draw gizmos
	inline virtual void OnDrawGizmos() {};

	static std::vector<Behaviour*> behaviourList;
};

