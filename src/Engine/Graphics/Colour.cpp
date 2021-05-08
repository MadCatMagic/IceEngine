#include "Colour.h"
#include <stdexcept>

Colour::Colour()
	: r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{ }

Colour::Colour(int r, int g, int b)
	: r((float)r / 255.0f), g((float)g / 255.0f), b((float)b / 255.0f), a(1.0f)
{ }

Colour::Colour(int r, int g, int b, int a)
	: r((float)r / 255.0f), g((float)g / 255.0f), b((float)b / 255.0f), a((float)a / 255.0f)
{ }

Colour::Colour(float r, float g, float b)
	: r(r), g(g), b(b), a(1.0f)
{ }

Colour::Colour(float r, float g, float b, float a)
	: r(r), g(g), b(b), a(a)
{ }

// colour should be of format "rrggbb" or "rrggbbaa"
Colour::Colour(const std::string& hexColour8)
{
	int ri, gi, bi, ai;
	int success = sscanf_s(hexColour8.c_str(), "%02x%02x%02x%02x", &ri, &gi, &bi, &ai);
	if (success == 4)
	{
		r = (float)ri / 255;
		g = (float)gi / 255;
		b = (float)bi / 255;
		a = (float)ai / 255;
	}
	else if (success == 3)
	{
		r = (float)ri / 255;
		g = (float)gi / 255;
		b = (float)bi / 255;
		a = 1.0f;
	}
	else
	{
		throw std::invalid_argument("String '" + hexColour8 + "' is an invalid hexadecimal colour.");
	}
}

Colour Colour::black =   Colour(0.0f, 0.0f, 0.0f, 1.0f);
Colour Colour::blue =    Colour(0.0f, 0.0f, 1.0f, 1.0f);
Colour Colour::clear =   Colour(0.0f, 0.0f, 0.0f, 0.0f);
Colour Colour::cyan =    Colour(0.0f, 1.0f, 1.0f, 1.0f);
Colour Colour::gray =    Colour(0.5f, 0.5f, 0.5f, 1.0f);
Colour Colour::green =   Colour(0.0f, 1.0f, 0.0f, 1.0f);
Colour Colour::grey =    Colour(0.5f, 0.5f, 0.5f, 1.0f);
Colour Colour::magenta = Colour(1.0f, 0.0f, 1.0f, 1.0f);
Colour Colour::red =     Colour(1.0f, 0.0f, 0.0f, 1.0f);
Colour Colour::white =   Colour(1.0f, 1.0f, 1.0f, 1.0f);
Colour Colour::yellow =  Colour(1.0f, 0.92f, 0.016f, 1.0f);