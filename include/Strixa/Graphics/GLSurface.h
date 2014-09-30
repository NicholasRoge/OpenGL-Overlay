#ifndef Strixa_Graphics_GLSurface_H
#define Strixa_Graphics_GLSurface_H

#include <GL/glew.h>
#include <memory>
#include <Strixa/Graphics/Surface.h>
#include <stdint.h>


namespace Strixa
{
	namespace Graphics
	{
		class GLSurface : public Surface
		{
			/* Member Properties */
			private:
				unsigned  height;
				uint32_t* texture_data;
				size_t    texture_data_size;
				GLuint    texture_handle;
				unsigned  width;

			/* Member Methods */
			public:
				GLSurface(unsigned width,unsigned height);

				GLSurface(const GLSurface& copy) = delete;

				GLSurface(GLSurface&& move);

				~GLSurface();

				void bind();

				virtual void drawLine(int from_x,int from_y,int to_x,int to_y);

				virtual void drawPixel(int x,int y);

				virtual void drawRect(int x,int y,unsigned width,unsigned height);

				virtual void fillRect(int x,int y,unsigned width,unsigned height);

				void flush();

				void resize(unsigned width,unsigned height);

				void unbind();
		};
	}
}

#endif