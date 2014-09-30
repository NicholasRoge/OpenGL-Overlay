#ifndef Strixa_GL_Shader_H
#define Strixa_GL_Shader_H

#include <GL/glew.h>
#include <string>


namespace Strixa
{
	namespace GL
	{
		class Shader
		{
			/* Member Properties */
			private:
				GLuint handle;

			/* Member Methods */
			public:
				Shader(GLenum type);

				bool compile();

				std::string getLog() const;

				std::string getSource() const;

				void setSource(nullptr_t);

				void setSource(const char* source);

				void setSourceFile(const char* path);

				operator GLuint() const;
		};
	}
}

#endif