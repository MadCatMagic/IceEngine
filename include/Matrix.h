#pragma once
#include <string>
#include "Vector.h"

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(float scalar);
	Matrix4x4(const Vector4& diagonal);
	Matrix4x4(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3);
	~Matrix4x4();

	void SetColumn(const Vector4& data, int column);
	void SetRow(const Vector4& data, int row);
	Vector4 GetColumn(int column);
	Vector4 GetRow(int row);
	float GetValue(int column, int row);
	void SetValue(int column, int row, float value);

	Matrix4x4 GetTranspose();
	Vector4 GetDiagonal();

	Matrix4x4 operator+(Matrix4x4 other) const;
	Matrix4x4 operator-(Matrix4x4 other) const;
	Matrix4x4 operator*(Matrix4x4 other) const;
	Vector4 operator*(Vector4 other) const;
	Matrix4x4 operator*(float scalar) const;
	std::string ToString();

	static Matrix4x4 PointAt(const Vector3& pos, const Vector3& target, const Vector3& up);
	static Matrix4x4 LookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

	static Matrix4x4 zero;
	static Matrix4x4 identity;

	float m[16] { };
	/* m = [
		 0,  1,  2,  3,
		 4,  5,  6,  7,
		 8,  9, 10, 11,
		12, 13, 14, 15
	]*/
};