#include <Strixa/UI/Button.h>

using Strixa::Graphics::Surface;
using Strixa::UI::Button;


/* Class [Strixa::UI::Button] Definition */
Button::Button(const std::string& text)
{
	this->text.assign(text);
}

const std::string& Button::getText() const
{
	return this->text;
}

void Button::onDraw(Surface& surface)
{
	surface.drawText(0,this->getHeight(),this->text);
}

void Button::setText(const std::string& text)
{
	this->text.assign(text);
}