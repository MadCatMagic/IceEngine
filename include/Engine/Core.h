#pragma once
// -- HEADER INCLUDES --
#include <string>
#include <vector>
#include <iostream>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

// -- TYPEDEFS --

// same functionality as std::string but using unsigned chars
// useful for binary file writing
typedef std::basic_string<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char>> ustring;

// -- USEFULL FUNCTIONS --

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

// converts a 4 byte value to a length 4 ustring
// for binary file writing
template<class b4>
ustring b4Toustring(b4 val) 
{
	// sketchy type hack to convert any 4b type into uint
	unsigned int uint = *(unsigned int*)((void*)&val);

	// bitshifting into place
	unsigned char bytes[4]{};
	bytes[0] = (uint >> 24) & 0xFF;
	bytes[1] = (uint >> 16) & 0xFF;
	bytes[2] = (uint >> 8) & 0xFF;
	bytes[3] = uint & 0xFF;
	
	// return as ustring
	return ustring(bytes, 4);
}

// converts a length 4 ustring to a 4 byte value
// for binary file writing
template<class b4>
b4 ustringTob4(ustring val)
{
	unsigned int uint = 
		((unsigned int)val[0] << 24) +
		((unsigned int)val[1] << 16) +
		((unsigned int)val[2] << 8) +
		(unsigned int)val[3];

	// return as type
	return *(b4*)((void*)&uint);
}

// converts a 8 byte value to a length 8 ustring
// for binary file writing
template<class b8>
ustring b8Toustring(b8 val)
{
	// sketchy type hack to convert any 4b type into uint
	unsigned long uint = *(unsigned long*)((void*)&val);

	// bitshifting into place
	unsigned char bytes[8]{};
	bytes[0] = (uint >> 56) & 0xFF;
	bytes[1] = (uint >> 48) & 0xFF;
	bytes[2] = (uint >> 40) & 0xFF;
	bytes[3] = (uint >> 32) & 0xFF;
	bytes[4] = (uint >> 24) & 0xFF;
	bytes[5] = (uint >> 16) & 0xFF; 
	bytes[6] = (uint >> 8) & 0xFF;
	bytes[7] = uint & 0xFF;

	// return as ustring
	return ustring(bytes, 8);
}

// converts a length 8 ustring to a 8 byte value
// for binary file writing
template<class b8>
b8 ustringTob8(ustring val)
{
	unsigned long uint =
		((unsigned long)val[0] << 56) +
		((unsigned long)val[1] << 48) +
		((unsigned long)val[2] << 40) +
		((unsigned long)val[3] << 32) +
		((unsigned long)val[4] << 24) +
		((unsigned long)val[5] << 16) +
		((unsigned long)val[6] << 8) +
		(unsigned long)val[7];

	// return as type
	return *(b8*)((void*)&uint);
}