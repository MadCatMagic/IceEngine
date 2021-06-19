#include "Engine/Core/Vector.h"

Vector4i::Vector4i()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
}

Vector4i::Vector4i(int x)
{
    this->x = x;
    this->y = 0;
    this->z = 0;
    this->w = 0;
}

Vector4i::Vector4i(int x, int y)
{
    this->x = x;
    this->y = y;
    this->z = 0;
    this->w = 0;
}

Vector4i::Vector4i(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 0;
}

Vector4i::Vector4i(int x, int y, int z, int w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4i Vector4i::operator+(const Vector4i& a) const
{
    return Vector4i(x + a.x, y + a.y, z + a.z, w + a.w);
}

Vector4i Vector4i::operator-() const
{
    return Vector4i(-x, -y, -z, -w);
}

Vector4i Vector4i::operator-(const Vector4i& a) const
{
    return Vector4i(x - a.x, y - a.y, z - a.z, w - a.w);
}

Vector4i Vector4i::operator*(int a) const
{
    return Vector4i(x * a, y * a, z * a, w * a);
}

Vector4i Vector4i::operator/(int a) const
{
    return Vector4i(x / a, y / a, z / a, w / a);
}

Vector4i& Vector4i::operator+=(const Vector4i& a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
    return *this;
}

Vector4i& Vector4i::operator-=(const Vector4i& a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
    return *this;
}

Vector4i& Vector4i::operator*=(int a)
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
    return *this;
}

Vector4i& Vector4i::operator/=(int a)
{
    x /= a;
    y /= a;
    z /= a;
    w /= a;
    return *this;
}

bool Vector4i::operator==(const Vector4i& a) const
{
    return x == a.x && y == a.y && z == a.z && w == a.w;
}

bool Vector4i::operator!=(const Vector4i& a) const
{
    return !(*this == a);
}

std::string Vector4i::ToString() const
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
}

Vector4i Vector4i::zero = Vector4i();
Vector4i Vector4i::one = Vector4i(1, 1, 1, 1);