#include <Strixa/Graphics/Colour.h>

using Strixa::Graphics::Colour;


/* Class [Strixa::Graphics::Colour] Definition */
const Colour Colour::BLACK(0,0,0,255);
const Colour Colour::WHITE(255,255,255,255);

Colour::Colour()
: red(((uint8_t*)&raw)[0]),green(((uint8_t*)&raw)[1]),blue(((uint8_t*)&raw)[2]),alpha(((uint8_t*)&raw)[3])
{
	this->raw = 0;
}

Colour::Colour(uint32_t colour)
: red(((uint8_t*)&raw)[0]),green(((uint8_t*)&raw)[1]),blue(((uint8_t*)&raw)[2]),alpha(((uint8_t*)&raw)[3])
{
	this->raw = colour;
}

Colour::Colour(uint8_t red,uint8_t green,uint8_t blue,uint8_t alpha)
: red(((uint8_t*)&raw)[0]),green(((uint8_t*)&raw)[1]),blue(((uint8_t*)&raw)[2]),alpha(((uint8_t*)&raw)[3])
{
	this->alpha = alpha;
	this->blue = blue;
	this->green = green;
	this->red = red;
}

Colour& Colour::operator =(const Colour& copy)
{
	this->raw = copy.raw;

	return *this;
}

Colour& Colour::operator =(Colour&& copy)
{
	this->raw = copy.raw;

	return *this;
}

Colour::operator uint32_t() const
{
	return this->raw;
}
