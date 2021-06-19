#include "Engine/Core/Ray.h"

Ray::Ray() { }

Ray::Ray(const Vector3& origin, const Vector3& direction)
{
    this->origin = origin;
    this->direction = direction;
}

Vector3 Ray::GetPoint(float distance) const
{
    return origin + direction * distance;
}

std::string Ray::ToString() const
{
    return "origin: " + origin.ToString() + ", dir: " + direction.ToString();
}
