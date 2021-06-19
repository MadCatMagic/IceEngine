#pragma once
// -- HEADER INCLUDES --
#include <string>
#include <vector>

#include "Core/Vector.h"
#include "Core/Matrix.h"
#include "Core/Quaternion.h"
#include "Core/Colour.h"
#include "Core/Ray.h"

// maybe put these here but idk
// would add a lot of compile time
//#include "Core/Time.h"
//#include "Core/Input.h"

// -- DEFINES --

// SafeDelete deletes a pointer if it isnt nullptr
#define SafeDelete(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }

// SafeDeleteArray deletes an array pointer if it isnt nullptr
#define SafeDeleteArray(ptr) if (ptr != nullptr) { delete[] ptr; ptr = nullptr; }

// -- TYPEDEFS --

// same functionality as std::string but using unsigned chars
// useful for binary file writing
typedef std::basic_string<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char>> ustring;

// -- CLASSES --

// auto deletes the object after deletion
// like std::unique_ptr
template<class T>
class safeptr
{
public:
	inline safeptr() { }
	inline safeptr(T* ptr) { obj = ptr; }
	inline ~safeptr() { SafeDelete(obj); }

	// releases the managed pointer
	inline void Release() { SafeDelete(obj); }

	// releases the current pointer and replaces it with ptr
	inline void Set(T* ptr) 
	{ 
		SafeDelete(obj);
		obj = ptr; 
	}

	// returns the current pointer and replaces it with ptr
	inline T* Reset(T* ptr) 
	{ 
		T* temp = obj; 
		obj = ptr; 
		return temp; 
	}

	// swaps the pointers between this and ptr
	inline void Swap(safeptr<T> ptr) 
	{ 
		T* temp = ptr.obj; 
		ptr.obj = obj;
		obj = temp; 
	}
	// swaps the pointers between this and ptr
	inline void Swap(T* ptr) 
	{ 
		T* temp = ptr; 
		ptr = obj; 
		obj = temp; 
	}

	inline T& operator*() { return *obj; }
	inline T* operator->() { return obj; }
	safeptr& operator=(safeptr&& a) noexcept { obj = a.obj; a.obj = nullptr; return this; }
	inline bool operator==(const safeptr<T>& a) { return obj == a.obj; }
	inline bool operator!=(const safeptr<T>& a) { return obj != a.obj; }
	
	inline T* Get() const { return obj; }

private:
	T* obj{ nullptr };
};

// -- USEFULL FUNCTIONS --

// COMPARISONS

// Clamp returns the value of x clamped to between a and b
template<class T>
inline T Clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }

// Max returns the maximum of a and b
// if they are equal will return a
template<class T>
inline T Max(T a, T b) { return a < b ? b : a; }

// Min returns the minimum of a and b
// if they are equal will return a
template<class T>
inline T Min(T a, T b) { return a > b ? b : a; }

// ARRAY MANIPULATIONS

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

// BYTE CONVERSIONS

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

// -- CONSOLE --
namespace Console 
{
	// dont use
	extern void _OnStart();
	// dont use
	extern void _OnExit();
	// logs a message to the console
	extern void Log(const std::string& message);
	// logs an error to the console
	extern void LogError(const std::string& message);
	// logs a warning to the console
	extern void LogWarning(const std::string& message);
}