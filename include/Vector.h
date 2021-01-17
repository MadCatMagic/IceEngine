#pragma once
#include <string>

struct Vector2;
struct Vector3;
struct Vector4;
struct Vector2i;
struct Vector3i;
struct Vector4i;

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x);
	Vector2(float x, float y);

	static float Dot(const Vector2& a, const Vector2& b);
	static float Magnitude(const Vector2& a);
	static Vector2 Normalize(const Vector2& a);
	
	Vector2 operator+(const Vector2& a) const;
	Vector2 operator-() const;
	Vector2 operator-(const Vector2& a) const;
	Vector2 operator*(float a) const;
	Vector2 operator/(float a) const;
	Vector2& operator+=(const Vector2& a);
	Vector2& operator-=(const Vector2& a);
	Vector2& operator*=(float a);
	Vector2& operator/=(float a);

	bool operator==(const Vector2& a) const;
	bool operator!=(const Vector2& a) const;
	
	std::string ToString() const;

	static Vector2 zero;
	static Vector2 one;
};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x);
	Vector3(float x, float y);
	Vector3(float x, float y, float z);
	Vector3(Vector4 wis1);

	static float Dot(const Vector3& a, const Vector3& b);
	static float Magnitude(const Vector3& a);
	static Vector3 Normalize(const Vector3& a);
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	Vector3 operator+(const Vector3& a) const;
	Vector3 operator-() const;
	Vector3 operator-(const Vector3& a) const;
	Vector3 operator*(float a) const;
	Vector3 operator/(float a) const;
	Vector3& operator+=(const Vector3& a);
	Vector3& operator-=(const Vector3& a);
	Vector3& operator*=(float a);
	Vector3& operator/=(float a);

	bool operator==(const Vector3& a) const;
	bool operator!=(const Vector3& a) const;
	
	std::string ToString() const;

	static Vector3 zero;
	static Vector3 one;
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float x);
	Vector4(float x, float y);
	Vector4(float x, float y, float z);
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 wtobe1);

	static float Dot(const Vector4& a, const Vector4& b);
	static float Magnitude(const Vector4& a);
	static Vector4 Normalize(const Vector4& a);

	Vector4 operator+(const Vector4& a) const;
	Vector4 operator-() const;
	Vector4 operator-(const Vector4& a) const;
	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;
	Vector4& operator+=(const Vector4& a);
	Vector4& operator-=(const Vector4& a);
	Vector4& operator*=(float a);
	Vector4& operator/=(float a);

	bool operator==(const Vector4& a) const;
	bool operator!=(const Vector4& a) const;

	std::string ToString() const;

	static Vector4 zero;
	static Vector4 one;
};

struct Vector2i
{
	int x;
	int y;

	Vector2i();
	Vector2i(int x);
	Vector2i(int x, int y);

	Vector2i operator+(const Vector2i& a) const;
	Vector2i operator-() const;
	Vector2i operator-(const Vector2i& a) const;
	Vector2i operator*(int a) const;
	Vector2i operator/(int a) const;
	Vector2i& operator+=(const Vector2i& a);
	Vector2i& operator-=(const Vector2i& a);
	Vector2i& operator*=(int a);
	Vector2i& operator/=(int a);

	bool operator==(const Vector2i& a) const;
	bool operator!=(const Vector2i& a) const;

	std::string ToString() const;

	static Vector2i zero;
	static Vector2i one;
};

struct Vector3i
{
	int x;
	int y;
	int z;

	Vector3i();
	Vector3i(int x);
	Vector3i(int x, int y);
	Vector3i(int x, int y, int z);

	Vector3i operator+(const Vector3i& a) const;
	Vector3i operator-() const;
	Vector3i operator-(const Vector3i& a) const;
	Vector3i operator*(int a) const;
	Vector3i operator/(int a) const;
	Vector3i& operator+=(const Vector3i& a);
	Vector3i& operator-=(const Vector3i& a);
	Vector3i& operator*=(int a);
	Vector3i& operator/=(int a);

	bool operator==(const Vector3i& a) const;
	bool operator!=(const Vector3i& a) const;

	std::string ToString() const;

	static Vector3i zero;
	static Vector3i one;
};

struct Vector4i
{
	int x;
	int y;
	int z;
	int w;

	Vector4i();
	Vector4i(int x);
	Vector4i(int x, int y);
	Vector4i(int x, int y, int z);
	Vector4i(int x, int y, int z, int w);

	Vector4i operator+(const Vector4i& a) const;
	Vector4i operator-() const;
	Vector4i operator-(const Vector4i& a) const;
	Vector4i operator*(int a) const;
	Vector4i operator/(int a) const;
	Vector4i& operator+=(const Vector4i& a);
	Vector4i& operator-=(const Vector4i& a);
	Vector4i& operator*=(int a);
	Vector4i& operator/=(int a);

	bool operator==(const Vector4i& a) const;
	bool operator!=(const Vector4i& a) const;

	std::string ToString() const;

	static Vector4i zero;
	static Vector4i one;
};