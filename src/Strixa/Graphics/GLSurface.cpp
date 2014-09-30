#include <Strixa/Graphics/GLSurface.h>

#include <algorithm>
#include <GL/GL.h>
#include <GL/GLU.h>

using Strixa::Graphics::GLSurface;


/* Class [Graphics::Surface] Definition */
GLSurface::GLSurface(unsigned width,unsigned height)
{
	this->texture_data = new uint32_t[0];
	glGenTextures(1,&this->texture_handle);

	this->resize(width,height);

	this->bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	this->unbind();
}

GLSurface::GLSurface(GLSurface&& move)
{
	this->texture_data_size = move.texture_data_size;
	this->texture_data = move.texture_data;
	this->texture_handle = move.texture_handle;
	this->height = move.height;
	this->width = move.width;

	move.texture_data = nullptr;
}

GLSurface::~GLSurface()
{
	if(this->texture_data != nullptr)
	{
		delete[] this->texture_data;
	}
}

void GLSurface::bind()
{
	glBindTexture(GL_TEXTURE_2D,this->texture_handle);
}

void GLSurface::drawLine(int from_x,int from_y,int to_x,int to_y)
{
	if(from_x < to_x)
	{
		std::swap(from_x,to_x);
		std::swap(from_y,to_y);
	}

	if(from_x == to_x)
	{
		for(;from_y < to_y;++from_y)
		{
			this->drawPixel(from_x,from_y);
		}
	}
	else
	{
		const int   delta_x = to_x - from_x;
		const int   delta_y = to_y - from_y;
		const float delta_error = std::abs((float)delta_x / (float)delta_y);

		float error;


		error = 0;
		for(int y = from_y;from_x <= to_x;++from_x)
		{
			this->drawPixel(from_x,y);
			
			error += delta_error;
			if(error >= 0.5f)
			{
				++y;
				error -= 1.0f;
			}
		}
	}
}

void GLSurface::drawPixel(int x,int y)
{
	this->texture_data[(((this->height - 1) - (this->getOffset().y + y)) * this->width) + (this->getOffset().x + x)] = this->getColour();
}

void GLSurface::drawRect(int x,int y,unsigned width,unsigned height)
{
	this->drawLine(x,y,x,y + height);                  //Left Line
	this->drawLine(x + width,y,x + width,y + height);  //Right Line
	this->drawLine(x,y,x + width,y);                   //Top Line
	this->drawLine(x,y + height,x + width,y + height); //Bottom Line
}

void GLSurface::fillRect(int x,int y,unsigned width,unsigned height)
{
	for(int y_offset = y;y_offset < height;++y_offset)
	{
		for(int x_offset = x;x_offset < width;++x_offset)
		{
			this->drawPixel(x_offset,y_offset);
		}
	}
}

void GLSurface::flush()
{
	this->bind();
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,this->width,this->height,GL_RGBA,GL_UNSIGNED_BYTE,this->texture_data);
	this->unbind();
}

void GLSurface::resize(unsigned width,unsigned height)
{
	delete[] this->texture_data;

	this->height = height;
	this->width = width;
	this->texture_data_size = this->height * this->width;
	this->texture_data = new uint32_t[this->texture_data_size];
	std::memset(this->texture_data,0,sizeof(uint32_t) * this->texture_data_size);

	this->bind();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
	this->unbind();
}

void GLSurface::unbind()
{
	glBindTexture(GL_TEXTURE_2D,0);
}