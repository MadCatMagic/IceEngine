#include "Engine/Core/Vector.h"

Vector2i::Vector2i()
{
    this->x = 0;
    this->y = 0;
}

Vector2i::Vector2i(int x)
{
    this->x = x;
    this->y = 0;
}

Vector2i::Vector2i(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2i Vector2i::operator+(const Vector2i& a) const
{
    return Vector2i(x + a.x, y + a.y);
}

Vector2i Vector2i::operator-() const
{
    return Vector2i(-x, -y);
}

Vector2i Vector2i::operator-(const Vector2i& a) const
{
    return Vector2i(x - a.x, y - a.y);
}

Vector2i Vector2i::operator*(int a) const
{
    return Vector2i(x * a, y * a);
}

Vector2i Vector2i::operator/(int a) const
{
    return Vector2i(x / a, y / a);
}

Vector2i& Vector2i::operator+=(const Vector2i& a)
{
    x += a.x;
    y += a.y;
    return *this;
}

Vector2i& Vector2i::operator-=(const Vector2i& a)
{
    x -= a.x;
    y -= a.y;
    return *this;
}

Vector2i& Vector2i::operator*=(int a)
{
    x *= a;
    y *= a;
    return *this;
}

Vector2i& Vector2i::operator/=(int a)
{
    x /= a;
    y /= a;
    return *this;
}

bool Vector2i::operator==(const Vector2i& a) const
{
    return x == a.x && y == a.y;
}

bool Vector2i::operator!=(const Vector2i& a) const
{
    return !(*this == a);
}

std::string Vector2i::ToString() const
{
    return std::to_string(x) + ", " + std::to_string(y);
}

Vector2i Vector2i::zero = Vector2i();
Vector2i Vector2i::one = Vector2i(1, 1);