#include <cmath>
#include "Engine/Vector.h"

Vector4::Vector4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Vector4::Vector4(const float x)
{
	this->x = x;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Vector4::Vector4(const float x, const float y)
{
	this->x = x;
	this->y = y;
	z = 0.0f;
	w = 0.0f;
}

Vector4::Vector4(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	w = 0.0f;
}

Vector4::Vector4(const float x, const float y, const float z, const float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(Vector3 wtobe1)
{
	this->x = wtobe1.x;
	this->y = wtobe1.y;
	this->z = wtobe1.z;
	this->w = 1.0f;
}

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float Vector4::Magnitude(const Vector4& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

static float kEpsilon = 0.00001f;
Vector4 Vector4::Normalize(const Vector4& a)
{
	float mag = Magnitude(a);
	if (mag > kEpsilon)
		return a / mag;
	else
		return zero;
}

Vector4 Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, w + a.w);
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
}

Vector4 Vector4::operator*(float a) const
{
	return Vector4(x * a, y * a, z * a, w * a);
}

Vector4 Vector4::operator/(float a) const
{
	return Vector4(x / a, y / a, z / a, w / a);
}

Vector4& Vector4::operator+=(const Vector4& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}

Vector4& Vector4::operator*=(float a)
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}

Vector4& Vector4::operator/=(float a)
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}

bool Vector4::operator==(const Vector4& a) const
{
	return x == a.x && y == a.y && z == a.z && w == a.w;
}

bool Vector4::operator!=(const Vector4& a) const
{
	return !operator==(a);
}

std::string Vector4::ToString() const
{
	return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
}

Vector4 Vector4::zero = Vector4();
Vector4 Vector4::one = Vector4(1.0f, 1.0f, 1.0f, 1.0f);