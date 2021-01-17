#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion()
{
    this->w = 1;
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

// z is yaw
// y is pitch
// x is roll
Quaternion::Quaternion(const Vector3& euler)
{
    double cy = cos(euler.z * 0.5);
    double sy = sin(euler.z * 0.5);
    double cp = cos(euler.y * 0.5);
    double sp = sin(euler.y * 0.5);
    double cr = cos(euler.x * 0.5);
    double sr = sin(euler.x * 0.5);

    this->w = (float)(cr * cp * cy + sr * sp * sy);
    this->x = (float)(sr * cp * cy - cr * sp * sy);
    this->y = (float)(cr * sp * cy + sr * cp * sy);
    this->z = (float)(cr * cp * sy - sr * sp * cy);
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
    this->w = cos(angle / 2);
    this->x = sin(angle / 2) * axis.x;
    this->y = sin(angle / 2) * axis.y;
    this->z = sin(angle / 2) * axis.z;
}

Quaternion::~Quaternion() { }

Quaternion Quaternion::operator+(const Quaternion& a) const
{
    return Quaternion(w + a.w, x + a.x, y + a.y, z + a.z);
}

Quaternion Quaternion::operator-() const
{
    return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::operator-(const Quaternion& a) const
{
    return Quaternion(w - a.w, x - a.x, y - a.y, z - a.z);
}

Quaternion Quaternion::operator*(const Quaternion& a) const
{
    return Quaternion(
        w * a.w - x * a.x - y * a.y - z * a.z,
        w * a.x + x * a.w + y * a.z - z * a.y,
        w * a.y - x * a.z + y * a.w + z * a.x,
        w * a.z - x * a.y - y * a.x - z * a.w
    );
}

Quaternion& Quaternion::operator+=(const Quaternion& a)
{
    this->w += a.w;
    this->x += a.x;
    this->y += a.y;
    this->z += a.z;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& a)
{
    this->w -= a.w;
    this->x -= a.x;
    this->y -= a.y;
    this->z -= a.z;
    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& a)
{
    this->x =  x * a.w + y * a.z - z * a.y + w * a.x;
    this->y = -x * a.z + y * a.w + z * a.x + w * a.y;
    this->z =  x * a.y - y * a.x + z * a.w + w * a.z;
    this->w = -x * a.x - y * a.y - z * a.z + w * a.w;
    return *this;
}

Matrix4x4 Quaternion::GetRotationMatrix() const
{
    Matrix4x4 mat = Matrix4x4();
    double w = this->w;
    double x = this->x;
    double y = this->y;
    double z = this->z;
    // calculate coefficients
    double x2 = x + x; double y2 = y + y;
    double z2 = z + z;
    double xx = x * x2; double xy = x * y2; double xz = x * z2;
    double yy = y * y2; double yz = y * z2; double zz = z * z2;
    double wx = w * x2; double wy = w * y2; double wz = w * z2;


    mat.m[0] = (float)(1.0 - (yy + zz)); mat.m[1] = (float)(xy - wz);
    mat.m[2] = (float)(xz + wy); mat.m[3] = 0.0;
    mat.m[4] = (float)(xy + wz); mat.m[5] = (float)(1.0 - (xx + zz));
    mat.m[6] = (float)(yz - wx); mat.m[7] = 0.0;
    mat.m[8] = (float)(xz - wy); mat.m[9] = (float)(yz + wx);
    mat.m[10] = (float)(1.0 - (xx + yy)); mat.m[11] = 0.0;
    mat.m[12] = 0; mat.m[14] = 0;
    mat.m[13] = 0; mat.m[15] = 1;
    return mat;
}

Vector3 Quaternion::RotateVector(const Vector3& a) const
{
    return Vector3(GetRotationMatrix() * Vector4(a));
}

Vector3 Quaternion::ToEuler() const
{
    double w = this->w;
    double x = this->x;
    double y = this->y;
    double z = this->z;
    return Vector3(
        (float)atan2(2.0 * (w * x + y * z), w * w - x * x - y * y + z * z),
        (float)-asin(2.0 * (x * z - w * y)),
        (float)atan2(2.0 * (w * z + x * y), w * w + x * x - y * y - z * z)
    );
}

float Quaternion::Length(const Quaternion& a)
{
    return 2.0f * (a.w + a.x + a.y + a.z);
}

Quaternion Quaternion::Normalize(const Quaternion& a)
{
    float n = (float)sqrt(a.w * a.w + a.x * a.x + a.y * a.y + a.z * a.z);
    if (n != 0.0f)
        return Quaternion(a.w / n, a.x / n, a.y / n, a.z / n);
    return Quaternion();
}

Quaternion Quaternion::Conjugate(const Quaternion& a)
{
    return Quaternion(a.w, -a.x, -a.y, -a.z);
}

bool Quaternion::operator==(const Quaternion& a) const
{
    return w == a.w && x == a.x && y == a.y && z == a.z;
}

bool Quaternion::operator!=(const Quaternion& a) const
{
    return !(*this == a);
}

Quaternion Quaternion::identity = Quaternion();