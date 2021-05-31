#pragma once
#include <string>

// like a vector4 except operations do not change the alpha value (for convieniance)
class Colour
{
public:
	Colour();
	Colour(int r, int g, int b);
	Colour(int r, int g, int b, int a);
	Colour(float r, float g, float b);
	Colour(float r, float g, float b, float a);
	Colour(const std::string& hexColour8);
	
	float r;
	float g;
	float b;
	float a;

	Colour operator+(const Colour& obj) const;
	Colour operator-(const Colour& obj) const;
	Colour operator*(float obj) const;
	Colour operator/(float obj) const;
	Colour& operator+=(const Colour& obj);
	Colour& operator-=(const Colour& obj);
	Colour& operator*=(float obj);
	Colour& operator/=(float obj);

	bool operator==(const Colour& obj) const;
	bool operator!=(const Colour& obj) const;

	std::string ToString() const;

	static Colour black;   // 0.0f, 0.0f, 0.0f, 1.0f
	static Colour blue;    // 0.0f, 0.0f, 1.0f, 1.0f
	static Colour clear;   // 0.0f, 0.0f, 0.0f, 0.0f
	static Colour cyan;    // 0.0f, 1.0f, 1.0f, 1.0f
	static Colour gray;    // 0.5f, 0.5f, 0.5f, 1.0f
	static Colour green;   // 0.0f, 1.0f, 0.0f, 1.0f
	static Colour grey;    // 0.5f, 0.5f, 0.5f, 1.0f
	static Colour magenta; // 1.0f, 0.0f, 1.0f, 1.0f
	static Colour red;     // 1.0f, 0.0f, 0.0f, 1.0f
	static Colour white;   // 1.0f, 1.0f, 1.0f, 1.0f
	static Colour yellow;  // 1.0f, 0.92f, 0.016f, 1.0f
};

