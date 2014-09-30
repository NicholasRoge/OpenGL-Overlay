#include <Strixa/Graphics/Surface.h>

#include <algorithm>

using Strixa::Graphics::Colour;
using Strixa::Graphics::Point;
using Strixa::Graphics::Surface;


/* Class [Graphics::Surface] Definition */
const Colour& Surface::getColour() const
{
	return this->colour;
}

const Point& Surface::getOffset() const
{
	return this->offset;
}

void Surface::setColour(const Colour& colour)
{
	this->colour = colour;
}

void Surface::setOffset(const Point& point)
{
	this->offset.x = point.x;
	this->offset.y = point.y;
}

void Surface::setOffset(int x,int y)
{
	this->offset.x = x;
	this->offset.y = y;
}

void Surface::translateOffset(int x,int y)
{
	this->offset.x += x;
	this->offset.y += y;
}