#ifndef Strixa_Graphics_Surface_H
#define Strixa_Graphics_Surface_H

#include <memory>
#include <Strixa/Graphics/Colour.h>
#include <Strixa/Graphics/Point.h>
#include <stdint.h>


namespace Strixa
{
	namespace Graphics
	{
		class Surface
		{
			/* Member Properties */
			protected:
				Colour colour;
				Point  offset;

			/* Member Methods */
			public:
				virtual void drawLine(int from_x,int from_y,int to_x,int to_y) = 0;

				virtual void drawPixel(int x,int y) = 0;

				virtual void drawRect(int x,int y,unsigned width,unsigned height) = 0;

				virtual void fillRect(int x,int y,unsigned width,unsigned height) = 0;

				const Colour& getColour() const;

				const Point& getOffset() const;

				virtual void setColour(const Colour& colour);

				virtual void setOffset(const Point& point);

				virtual void setOffset(int x,int y);

				virtual void translateOffset(int x,int y);
		};
	}
}

#endif