#include <cmath>
#include "Engine/Vector.h"

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x)
{
	this->x = x;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x, float y)
{
	this->x = x;
	this->y = y;
	z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(Vector4 wis1)
{
	this->x = wis1.x;
	this->y = wis1.y;
	this->z = wis1.z;
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vector3::Magnitude(const Vector3& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

static float kEpsilon = 0.00001f;
Vector3 Vector3::Normalize(const Vector3& a)
{
	float mag = Magnitude(a);
	if (mag > kEpsilon)
		return a / mag;
	else
		return zero;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

Vector3 Vector3::operator+(const Vector3& a) const
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-(const Vector3& a) const
{
	return Vector3(x - a.x, y - a.y, z - a.z);
}

Vector3 Vector3::operator*(float a) const
{
	return Vector3(x * a, y * a, z * a);
}

Vector3 Vector3::operator/(float a) const
{
	return Vector3(x / a, y / a, z / a);
}

Vector3& Vector3::operator+=(const Vector3& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

Vector3& Vector3::operator*=(float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

Vector3& Vector3::operator/=(float a)
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}

bool Vector3::operator==(const Vector3& a) const
{
	return x == a.x && y == a.y && z == a.z;
}

bool Vector3::operator!=(const Vector3& a) const
{
	return !operator==(a);
}

std::string Vector3::ToString() const
{
	return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

Vector3 Vector3::zero = Vector3();
Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);