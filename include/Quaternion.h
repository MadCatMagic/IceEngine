#pragma once
#include "Matrix.h"
#include <string>

// mostly from http://www.chrobotics.com/library/understanding-quaternions
// complicated maths D:
class Quaternion
{
public:
	float w;
	float x;
	float y;
	float z;

	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Vector3& euler);
	Quaternion(const Vector3& axis, float angle);
	~Quaternion();

	Quaternion operator+(const Quaternion& a) const;
	Quaternion operator-() const;
	Quaternion operator-(const Quaternion& a) const;
	Quaternion operator*(const Quaternion& a) const;
	Quaternion operator/(const Quaternion& a) const;
	Quaternion& operator+=(const Quaternion& a);
	Quaternion& operator-=(const Quaternion& a);
	Quaternion& operator*=(const Quaternion& a);

	Matrix4x4 GetRotationMatrix() const;
	Vector3 RotateVector(const Vector3& a) const;
	Vector3 ToEuler() const;

	static float Length(const Quaternion& a);
	static Quaternion Normalize(const Quaternion& a);
	static Quaternion Conjugate(const Quaternion& a);

	bool operator==(const Quaternion& a) const;
	bool operator!=(const Quaternion& a) const;

	std::string ToString() const;

	static Quaternion identity;
};