#include "Engine/Vector.h"

Vector3i::Vector3i()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3i::Vector3i(int x)
{
    this->x = x;
    this->y = 0;
    this->z = 0;
}

Vector3i::Vector3i(int x, int y)
{
    this->x = x;
    this->y = y;
    this->z = 0;
}

Vector3i::Vector3i(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3i Vector3i::operator+(const Vector3i& a) const
{
    return Vector3i(x + a.x, y + a.y, z + a.z);
}

Vector3i Vector3i::operator-() const
{
    return Vector3i(-x, -y, -z);
}

Vector3i Vector3i::operator-(const Vector3i& a) const
{
    return Vector3i(x - a.x, y - a.y, z - a.z);
}

Vector3i Vector3i::operator*(int a) const
{
    return Vector3i(x * a, y * a, z * a);
}

Vector3i Vector3i::operator/(int a) const
{
    return Vector3i(x / a, y / a, z / a);
}

Vector3i& Vector3i::operator+=(const Vector3i& a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
}

Vector3i& Vector3i::operator-=(const Vector3i& a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
}

Vector3i& Vector3i::operator*=(int a)
{
    x *= a;
    y *= a;
    z *= a;
    return *this;
}

Vector3i& Vector3i::operator/=(int a)
{
    x /= a;
    y /= a;
    z /= a;
    return *this;
}

bool Vector3i::operator==(const Vector3i& a) const
{
    return x == a.x && y == a.y && z == a.z;
}

bool Vector3i::operator!=(const Vector3i& a) const
{
    return !(*this == a);
}

std::string Vector3i::ToString() const
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

Vector3i Vector3i::zero = Vector3i();
Vector3i Vector3i::one = Vector3i(1, 1, 1);