#include <Strixa/UI/Frame.h>

#include <memory>

using Strixa::Graphics::Surface;
using Strixa::UI::Frame;
using Strixa::UI::Pane;


/* Class [Strixa::UI::Frame] Definition */
Frame::Frame()
{
	this->children.reserve(64);
}

void Frame::draw(Surface& surface)
{
	if(this->isInvalidated())
	{
		this->Pane::draw(surface);

		for(Pane* pane : this->children)
		{
			surface.translateOffset(pane->getX(),pane->getY());
			pane->draw(surface);
			surface.translateOffset(-pane->getX(),-pane->getY());
		}
	}
}

void Frame::removeChild(size_t cpo)
{
	if(cpo < 0 || cpo >= this->children.size())
	{
		throw std::exception();
	}

	this->children.erase(this->children.begin() + cpo);  //TODO:  I think erase shifts the elements to the right of cpo left one.  Make sure of this.
}

void Frame::onChildMoveRequest(Pane& child,int x,int y)
{
	if(!child.hasParent() || child.getParent() != this)
	{
		throw std::exception();  //TODO:  Invalid argument
	}

	child.onMove(x,y);
}

void Frame::onChildResizeRequest(Pane& child,unsigned width,unsigned height)
{
	if(!child.hasParent() || child.getParent() != this)
	{
		throw std::exception();  //TODO:  Invalid argument
	}

	child.onResize(width,height);
}

/*LayoutManager& getLayoutManager() const
{

}

void setLayoutManager(const LayoutManager& manager)
{

}*/

Pane& Frame::operator [](size_t cpo)
{
	if(cpo < 0 || cpo >= this->children.size())
	{
		throw std::exception();
	}

	return *this->children[cpo];
}