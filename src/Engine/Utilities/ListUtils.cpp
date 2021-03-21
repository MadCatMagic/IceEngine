#include "ListUtils.h"

int Sum(const std::list<int>& l)
{
	int s = 0;
	for (int v : l)
	{
		s += v;
	}
	return s;
}

unsigned int Sum(const std::list<unsigned int>& l)
{
	unsigned int s = 0;
	for (unsigned int v : l)
	{
		s += v;
	}
	return s;
}