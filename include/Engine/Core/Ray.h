#pragma once
#include "Vector.h"

class Ray
{
public:
	Ray();
	Ray(const Vector3& origin, const Vector3& direction);

	Vector3 origin;
	Vector3 direction;

	// returns a point at a distance along the ray
	Vector3 GetPoint(float distance) const;

	std::string ToString() const;
};

