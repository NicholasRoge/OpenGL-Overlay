#ifndef Strix_Graphics_Colour_H
#define Strixa_Graphics_Colour_H

#include <stdint.h>


namespace Strixa
{
	namespace Graphics
	{
		class Colour
		{
			/* Class Properties */
			public:
				static const Colour BLACK;
				static const Colour WHITE;

			/* Member Properties */
			private:
				uint32_t raw;

			public:
				uint8_t& alpha;
				uint8_t& blue;
				uint8_t& green;
				uint8_t& red;

			/* Member Methods */
			public:
				Colour();

				Colour(uint32_t colour);

				Colour(uint8_t red,uint8_t green,uint8_t blue,uint8_t alpha);

				Colour& operator =(const Colour& copy);

				Colour& operator =(Colour&& copy);

				operator uint32_t() const;
		};
	}
}

#endif