#pragma once
#include <string>
#include <vector>

// matrix includes
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

// useful functions

// Clamp returns the value of x clamped to between a and b
template<class T>
inline T Clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }

// StrSplit splits up string str by token splitBy, and appends the result to tokens
void StrSplit(std::string str, std::string splitBy, std::vector<std::string>& tokens);

// Sum adds up the elements (type T) of some container L
template<class T, class L>
T Sum(const L& l)
{
	T s = (T)0;
	for (T v : l)
		s += v;
	return s;
}