#include "Engine/Core/Vector.h"
#include <cmath>

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x)
{
	this->x = x;
	y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Magnitude(const Vector2& a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float Vector2::Distance(const Vector2& a, const Vector2& b)
{
	return Magnitude(a - b);
}

static float kEpsilon = 0.00001f;
Vector2 Vector2::Normalize(const Vector2& a)
{
	float mag = Magnitude(a);
	if (mag > kEpsilon)
		return a / mag;
	else
		return zero;
}

Vector2 Vector2::operator+(const Vector2& a) const
{
	return Vector2(x + a.x, y + a.y);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator-(const Vector2& a) const
{
	return Vector2(x - a.x, y - a.y);
}

Vector2 Vector2::operator*(float a) const
{
	return Vector2(x * a, y * a);
}

Vector2 Vector2::operator/(float a) const
{
	return Vector2(x / a, y / a);
}

Vector2& Vector2::operator+=(const Vector2& a)
{
	x += a.x;
	y += a.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}

Vector2& Vector2::operator*=(float a)
{
	x *= a;
	y *= a;
	return *this;
}

Vector2& Vector2::operator/=(float a)
{
	x /= a;
	y /= a;
	return *this;
}

bool Vector2::operator==(const Vector2& a) const
{
	return x == a.x && y == a.y;
}

bool Vector2::operator!=(const Vector2& a) const
{
	return !operator==(a);
}

std::string Vector2::ToString() const
{
	return std::to_string(x) + ", " + std::to_string(y);
}

Vector2 Vector2::zero = Vector2();
Vector2 Vector2::one = Vector2(1.0f, 1.0f);