#include <Strixa/GL/Shader.h>

#include <fstream>
#include <GL\GL.h>
#include <GL\GLU.h>

using Strixa::GL::Shader;


/* Class [Strixa::GL::Shader] Definition */
Shader::Shader(GLenum type)
{
	this->handle = glCreateShader(type);
}

/*std::string Shader::getLog() const
{
	std::string log;
	GLint       log_length;


	glGetShaderiv(this->handle,GL_INFO_LOG_LENGTH,&log_length);
	if(log_length == 0)
	{
		log.reserve(log_length);
		glGetShaderInfoLog(this->handle,log_length,nullptr,&(log[0]));
	}

	return log;
}*/

void Shader::setSource(nullptr_t)
{
	//TODO:  Throw an exception maybe?  I'm really not sure.
}

void Shader::setSource(const char* source)
{
	glShaderSource(this->handle,1,&source,nullptr);
}

/*void Shader::setSourceFile(const char* path)
{
	char*        source;
	std::fstream stream;


	stream.open(path,std::ios_base::in);
	stream.seekg();
}*/

Shader::operator GLuint() const
{
	return this->handle;
}