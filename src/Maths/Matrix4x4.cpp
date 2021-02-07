

#include "Matrix.h"

Matrix4x4::Matrix4x4() { }

Matrix4x4::Matrix4x4(float scalar)
{
	m[0] = scalar;
	m[5] = scalar;
	m[10] = scalar;
	m[15] = scalar;
}

Matrix4x4::Matrix4x4(const Vector4& diagonal)
{
	m[0] = diagonal.x;
	m[5] = diagonal.y;
	m[10] = diagonal.z;
	m[15] = diagonal.w;
}

Matrix4x4::Matrix4x4(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
{
	float M[16] = {
		c0.x, c1.x, c2.x, c3.x,
		c0.y, c1.y, c2.y, c3.y,
		c0.z, c1.z, c2.z, c3.z,
		c0.w, c1.w, c2.w, c3.w
	};
	for (int i = 0; i < 16; i++)
	{
		m[i] = M[i];
	}
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::SetColumn(const Vector4& data, int column)
{
	m[0 + column] = data.x;
	m[4 + column] = data.y;
	m[8 + column] = data.z;
	m[12 + column] = data.w;
}

void Matrix4x4::SetRow(const Vector4& data, int row)
{
	m[0 + row * 4] = data.x;
	m[1 + row * 4] = data.y;
	m[2 + row * 4] = data.z;
	m[3 + row * 4] = data.w;
}

Vector4 Matrix4x4::GetColumn(int column)
{
	return Vector4(m[0 + column], m[4 + column], m[8 + column], m[12 + column]);
}

Vector4 Matrix4x4::GetRow(int row)
{
	return Vector4(m[0 + row * 4], m[1 + row * 4], m[2 + row * 4], m[3 + row * 4]);
}

float Matrix4x4::GetValue(int column, int row)
{
	return m[column + row * 4];
}

void Matrix4x4::SetValue(int column, int row, float value)
{
	m[column + row * 4] = value;
}

Matrix4x4 Matrix4x4::GetTranspose()
{
	Matrix4x4 M = Matrix4x4();
	M.m[0] = m[0];
	M.m[1] = m[4];
	M.m[2] = m[8];
	M.m[3] = m[12];
	M.m[4] = m[1];
	M.m[5] = m[5];
	M.m[6] = m[9];
	M.m[7] = m[13];
	M.m[8] = m[2];
	M.m[9] = m[6];
	M.m[10] = m[10];
	M.m[11] = m[14];
	M.m[12] = m[3];
	M.m[13] = m[7];
	M.m[14] = m[11];
	return M;
}

Vector4 Matrix4x4::GetDiagonal()
{
	return Vector4(m[0], m[5], m[10], m[15]);
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4 other) const
{
	Matrix4x4 M = Matrix4x4();
	M.m[0] = m[0] + other.m[0];
	M.m[1] = m[1] + other.m[1];
	M.m[2] = m[2] + other.m[2];
	M.m[3] = m[3] + other.m[3];
	M.m[4] = m[4] + other.m[4];
	M.m[5] = m[5] + other.m[5];
	M.m[6] = m[6] + other.m[6];
	M.m[7] = m[7] + other.m[7];
	M.m[8] = m[8] + other.m[8];
	M.m[9] = m[9] + other.m[9];
	M.m[10] = m[10] + other.m[10];
	M.m[11] = m[11] + other.m[11];
	M.m[12] = m[12] + other.m[12];
	M.m[13] = m[13] + other.m[13];
	M.m[14] = m[14] + other.m[14];
	M.m[15] = m[15] + other.m[15];
	return M;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4 other) const
{
	Matrix4x4 M = Matrix4x4();
	M.m[0] = m[0] - other.m[0];
	M.m[1] = m[1] - other.m[1];
	M.m[2] = m[2] - other.m[2];
	M.m[3] = m[3] - other.m[3];
	M.m[4] = m[4] - other.m[4];
	M.m[5] = m[5] - other.m[5];
	M.m[6] = m[6] - other.m[6];
	M.m[7] = m[7] - other.m[7];
	M.m[8] = m[8] - other.m[8];
	M.m[9] = m[9] - other.m[9];
	M.m[10] = m[10] - other.m[10];
	M.m[11] = m[11] - other.m[11];
	M.m[12] = m[12] - other.m[12];
	M.m[13] = m[13] - other.m[13];
	M.m[14] = m[14] - other.m[14];
	M.m[15] = m[15] - other.m[15];
	return M;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 other) const
{
	Matrix4x4 M = Matrix4x4();
	M.m[0] = other.m[0] * m[0] + other.m[4] * m[1] + other.m[8] * m[2] + other.m[12] * m[3];
	M.m[1] = other.m[1] * m[0] + other.m[5] * m[1] + other.m[9] * m[2] + other.m[13] * m[3];
	M.m[2] = other.m[2] * m[0] + other.m[6] * m[1] + other.m[10] * m[2] + other.m[14] * m[3];
	M.m[3] = other.m[3] * m[0] + other.m[7] * m[1] + other.m[11] * m[2] + other.m[15] * m[3];

	M.m[4] = other.m[0] * m[4] + other.m[4] * m[5] + other.m[8] * m[6] + other.m[12] * m[7];
	M.m[5] = other.m[1] * m[4] + other.m[5] * m[5] + other.m[9] * m[6] + other.m[13] * m[7];
	M.m[6] = other.m[2] * m[4] + other.m[6] * m[5] + other.m[10] * m[6] + other.m[14] * m[7];
	M.m[7] = other.m[3] * m[4] + other.m[7] * m[5] + other.m[11] * m[6] + other.m[15] * m[7];

	M.m[8] = other.m[0] * m[8] + other.m[4] * m[9] + other.m[8] * m[10] + other.m[12] * m[11];
	M.m[9] = other.m[1] * m[8] + other.m[5] * m[9] + other.m[9] * m[10] + other.m[13] * m[11];
	M.m[10] = other.m[2] * m[8] + other.m[6] * m[9] + other.m[10] * m[10] + other.m[14] * m[11];
	M.m[11] = other.m[3] * m[8] + other.m[7] * m[9] + other.m[11] * m[10] + other.m[15] * m[11];

	M.m[12] = other.m[0] * m[12] + other.m[4] * m[13] + other.m[8] * m[14] + other.m[12] * m[15];
	M.m[13] = other.m[1] * m[12] + other.m[5] * m[13] + other.m[9] * m[14] + other.m[13] * m[15];
	M.m[14] = other.m[2] * m[12] + other.m[6] * m[13] + other.m[10] * m[14] + other.m[14] * m[15];
	M.m[15] = other.m[3] * m[12] + other.m[7] * m[13] + other.m[11] * m[14] + other.m[15] * m[15];
	return M;
}

Vector4 Matrix4x4::operator*(const Vector4 other) const
{
	Vector4 v = Vector4();
	v.x = other.x * m[0] + other.y * m[4] + other.z * m[8] + other.w * m[12];
	v.y = other.x * m[1] + other.y * m[5] + other.z * m[9] + other.w * m[13];
	v.z = other.x * m[2] + other.y * m[6] + other.z * m[10] + other.w * m[14];
	v.w = other.x * m[3] + other.y * m[7] + other.z * m[11] + other.w * m[15];
	return v;
}

Vector3 Matrix4x4::operator*(Vector3 other) const
{
	return Vector3(*this * Vector4(other));
}

Matrix4x4 Matrix4x4::operator*(float scalar) const
{
	Matrix4x4 M = Matrix4x4();
	M.m[0] = m[0] * scalar;
	M.m[1] = m[1] * scalar;
	M.m[2] = m[2] * scalar;
	M.m[3] = m[3] * scalar;
	M.m[4] = m[4] * scalar;
	M.m[5] = m[5] * scalar;
	M.m[6] = m[6] * scalar;
	M.m[7] = m[7] * scalar;
	M.m[8] = m[8] * scalar;
	M.m[9] = m[9] * scalar;
	M.m[10] = m[10] * scalar;
	M.m[11] = m[11] * scalar;
	M.m[12] = m[12] * scalar;
	M.m[13] = m[13] * scalar;
	M.m[14] = m[14] * scalar;
	M.m[15] = m[15] * scalar;
	return M;
}

std::string Matrix4x4::ToString()
{
	std::string str = "";
	for (int column = 0; column < 4; column++) 
	{
		for (int row = 0; row < 4; row++)
		{
			str += std::to_string(m[column + row * 4]) + "\t";
		}
		str += "\n";
	}
	return str;
}

Matrix4x4 Matrix4x4::PointAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 newForward = Vector3::Normalize(target - pos);
	Vector3 a = newForward * Vector3::Dot(up, newForward);
	Vector3 newUp = Vector3::Normalize(up - a);
	Vector3 newRight = Vector3::Cross(newUp, newForward);
	Matrix4x4 matrix = Matrix4x4(
		Vector4(newRight.x, newRight.y, newRight.z, 0.0f),
		Vector4(newUp.x, newUp.y, newUp.z, 0.0f),
		Vector4(newForward.x, newForward.y, newForward.z, 0.0f),
		Vector4(pos.x, pos.y, pos.z, 1.0f)
	);
	return matrix;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 newForward = Vector3::Normalize(target - pos);
	Vector3 a = newForward * Vector3::Dot(up, newForward);
	Vector3 newUp = Vector3::Normalize(up - a);
	Vector3 newRight = Vector3::Cross(newUp, newForward);

	/*
	Matrix4x4 matrix = Matrix4x4(
		Vector4(newRight.x,   newRight.y,   newRight.z,   -Vector3::Dot(pos, newRight)),
		Vector4(newUp.x,      newUp.y,      newUp.z,      -Vector3::Dot(pos, newUp)),
		Vector4(newForward.x, newForward.y, newForward.z, -Vector3::Dot(pos, newForward)),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);*/
	
	Matrix4x4 matrix = Matrix4x4(
		Vector4(newRight.x, newUp.x, newForward.x, 0.0f),
		Vector4(newRight.y, newUp.y, newForward.y, 0.0f),
		Vector4(newRight.z, newUp.z, newForward.z, 0.0f),
		Vector4(-Vector3::Dot(pos, newRight), -Vector3::Dot(pos, newUp), -Vector3::Dot(pos, newForward), 1.0f)
	);
	return matrix;
}

Matrix4x4 Matrix4x4::Rotation(const Vector3& euler)
{
	float sx = (float)sin(euler.x);
	float sy = (float)sin(euler.y);
	float sz = (float)sin(euler.z);
	float cx = (float)cos(euler.x);
	float cy = (float)cos(euler.y);
	float cz = (float)cos(euler.z);

	return Matrix4x4(
		Vector4(cz * cx - sz * sx * sy, -cx * sz, cz * sy + cy * sz * sx, 0.0f),
		Vector4(cy * sz + cz * sx * sy, cz * cx, sz * sy - cz * cy * sx, 0.0f),
		Vector4(-cx * sy, sx, cx * cy, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::zero = Matrix4x4();

Matrix4x4 Matrix4x4::identity = Matrix4x4(1.0f);

