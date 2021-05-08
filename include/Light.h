#pragma once
#include <vector>

#include "Colour.h"
#include "Vector.h"
#include "Behaviour.h"

class Light : public Behaviour
{
public:
	enum class Type {
		Point, Sun
	};

	Light();
	~Light();

	float strength{ 1.0f };
	Colour colour{ Colour::white };
	Type type{ Type::Sun };

	Matrix4x4 GetMatrix();

	inline std::vector<Light*>* GetLights() { return &lights; }

private:
	static std::vector<Light*> lights;
};