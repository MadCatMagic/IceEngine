#pragma once

namespace FMaths
{
	template<class T>
	inline T Clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }
}