#include <Strixa/UI/Pane.h>

#include <Strixa/UI/Frame.h>

using Strixa::Graphics::Colour;
using Strixa::Graphics::Surface;
using Strixa::UI::Frame;
using Strixa::UI::Pane;


/* Class [Strixa::UI::Pane] Definition */            
Pane::Pane()
{
	this->background_colour = Colour::WHITE;
	this->height = 600;
	this->invalidated = true;
	this->name = "";
	this->pos_x = 0;
	this->pos_y = 0;
	this->width = 800;
}
            
Pane::Pane(Pane&& pane)
{
	this->background_colour = background_colour;
	this->height = pane.height;
	this->invalidated = pane.invalidated;
	this->name.assign(pane.name);
	this->pos_x = pane.pos_x;
	this->pos_y = pane.pos_y;
	this->width = pane.width;
}

Pane::~Pane()
{
}

void Pane::draw(Surface& surface)
{
	surface.setColour(this->background_colour);
	surface.fillRect(0,0,this->width,this->height);

	this->invalidated = false;
}

const Colour& Pane::getBackgroundColour() const
{
	return this->background_colour;
}

unsigned Pane::getHeight() const
{
	return this->height;
}

Frame* Pane::getParent() const
{
	return this->parent;
}

unsigned Pane::getWidth() const
{
	return this->width;
}

int Pane::getX() const
{
	return this->pos_x;
}

int Pane::getY() const
{
	return this->pos_y;
}

bool Pane::hasParent() const
{
	return this->parent != nullptr;
}

void Pane::invalidate()
{
	if(this->hasParent())
	{
		this->getParent()->invalidate();
	}

	this->invalidated = true;
}

bool Pane::isInvalidated() const
{
	return this->invalidated;
}

void Pane::onDraw(Surface& surface)
{
}

void Pane::onMove(int x,int y)
{
	this->pos_x = x;
	this->pos_y = y;

	this->invalidate();
}

void Pane::onResize(unsigned width,unsigned height)
{
	this->width = width;
	this->height = height;

	this->invalidate();
}

void Pane::move(int x,int y)
{
	if(this->hasParent())
	{
		this->getParent()->onChildMoveRequest(*this,x,y);
	}
	else
	{
		this->onMove(x,y);
	}
}

void Pane::resize(unsigned width,unsigned height)
{
	if(this->hasParent())
	{
		this->getParent()->onChildResizeRequest(*this,width,height);
	}
	else
	{
		this->onResize(width,height);
	}
}

void Pane::setBackgroundColour(const Colour& colour)
{
	this->background_colour = colour;
}