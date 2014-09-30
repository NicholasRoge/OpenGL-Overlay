#ifndef Strixa_UI_Pane_H
#define Strixa_UI_Pane_H

#include <string>
//#include <Strixa/Graphics/Rectangle.h>
#include <Strixa/Graphics/Surface.h>


namespace Strixa
{
	namespace UI
	{
		class Pane
		{
			friend class Frame;
			
			public:
				/*typedef Strixa::Graphics::Rectangle Margin;
				typedef Strixa::Graphics::Rectangle Padding;*/

			/* Member Properties */
			private:
				Graphics::Colour background_colour;
				unsigned         height;
				bool             invalidated;
				std::string      name;
				Frame*           parent;
				int              pos_x;
				int              pos_y;
				unsigned         width;
            
			/* Member Methods */
			private:
				virtual void onDraw(Graphics::Surface& surface);
			
			protected:
				/**
				 * Draws this Pane to the specified surface.
				 * Note:  Classes that wish to inherit from the Pane class should only override this method if they have very good reason, and if they do, they must make a call to their parent draw method.  For general drawing needs, override Pane#onDraw(Surface&).
				 *
				 * @param surface Surface that the pane should draw itself onto.
				 */
				virtual void draw(Graphics::Surface& surface);

				virtual void onMove(int x,int y);

				virtual void onResize(unsigned width,unsigned height);
            
			public:
				Pane();
            
				Pane(const Pane& pane) = delete;

				Pane(Pane&& pane);

				virtual ~Pane();

				virtual const Graphics::Colour& getBackgroundColour() const;

				virtual unsigned getHeight() const;

				Frame* getParent() const;

				virtual unsigned getWidth() const;

				virtual int getX() const;

				virtual int getY() const;

				virtual bool hasParent() const;

				virtual void invalidate();

				virtual bool isInvalidated() const;

				virtual void move(int x,int y);

				virtual void resize(unsigned width,unsigned height);

				virtual void setBackgroundColour(const Graphics::Colour& colour);
		};
	}
}

#endif