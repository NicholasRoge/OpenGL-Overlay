
#include <GLWindow.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <iostream>
#include <Strixa/GL/Shader.h>

using Strixa::Graphics::GLSurface;
using Strixa::Graphics::Surface;
using Strixa::GL::Shader;


/* Class [GLWindow] Definition */
GLWindow::GLWindow()
{
	this->device_context_handle = nullptr;
	this->render_context_handle = nullptr;
}

void GLWindow::destroy()
{
	/* Deactive the rendering context. */
	if(this->render_context_handle != nullptr)
	{
		if(!wglMakeCurrent(nullptr,nullptr))
		{
			/*LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			std::clog
				<< "[Error]" << std::endl
				<< "  Unable to deactivate render context." << std::endl
				<< "  Cause:  " << error_message << std::endl;
			LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/
		}

		if(!wglDeleteContext(this->render_context_handle))
		{
			/*LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			std::clog
				<< "[Error]" << std::endl
				<< "  Unable to delete render context." << std::endl
				<< "  Cause:  " << error_message << std::endl;
			LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/
		}
	}
	this->render_context_handle = nullptr;

	if(this->device_context_handle != nullptr && !ReleaseDC(*this,this->device_context_handle))
	{
		/*LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to release device context." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/
	}
	this->device_context_handle = nullptr;

	this->Window::destroy();
}

WNDCLASSEX GLWindow::getWindowClass() const
{
	WNDCLASSEX window_class;

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandle(nullptr);
	window_class.hIcon = LoadIcon(nullptr,IDI_APPLICATION);
	window_class.hCursor = LoadCursor(nullptr,IDC_ARROW);
	window_class.hbrBackground = nullptr;
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = L"OpenGL Overlay";
	window_class.hIconSm = LoadIcon(nullptr,IDI_APPLICATION);
	
	return window_class;
}

LRESULT GLWindow::handleMessage(UINT message,WPARAM w_param,LPARAM l_param)
{
	switch(message)
	{
		case WM_PAINT:
			{
				return DefWindowProc(*this,message,w_param,l_param);
			}
	}
	
	return this->Window::handleMessage(message,w_param,l_param);
}

bool GLWindow::initDeviceContext()
{
	this->device_context_handle = GetDC(*this);
	if(this->device_context_handle == nullptr)
	{
		/*LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to retrieve a device context." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?

		return false;*/
	}

	return true;
}

bool GLWindow::initGL()
{
	static const float vertices[] = {
		-1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,0.0f
	};

	static GLchar* vertex_shader_source =
		"#version 150\n"
		"\n"
		"in vec2 position;\n"
		"in vec2 vertex_uv;\n"
		"out vec2 fragment_uv;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position.x,position.y,0.0,1.0);\n"
		"    fragment_uv = vertex_uv;\n"
		"}";

	static GLchar* fragment_shader_source =
		"#version 150\n"
		"\n"
		"out vec4 outColor;\n"
		"uniform sampler2D texture_sampler;\n"
		"in vec2 fragment_uv;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    outColor = texture(texture_sampler,fragment_uv).rgba;\n"
		"}";


	/*  */
	{
		GLenum status;


		status = glewInit();
		if(status != GLEW_OK)
		{
			std::clog
				<< "[Error]" << std::endl
				<< "  Unable to retrieve a device context." << std::endl
				<< "  Cause:  " << glewGetErrorString(status) << std::endl;
			
			return false;
		}
	}

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	GLuint vao;
	{
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);
	}

	GLuint vbo;
	{
		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STREAM_DRAW);
	}

	/* Create the surface that all panes will draw onto. */
	this->surface = std::make_unique<GLSurface>(this->getWidth(),this->getHeight());

	GLuint program;
	{
		program = glCreateProgram();


		GLuint shader;
		GLint  shader_status;
		{
			shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(shader,1,&vertex_shader_source,nullptr);
			glCompileShader(shader);
			glGetShaderiv(shader,GL_COMPILE_STATUS,&shader_status);
			if(shader_status == GL_TRUE)
			{
				glAttachShader(program,shader);
			}
			else
			{
				char buffer[2048];


				
				std::clog
					<< "[Error]" << std::endl
					<< "  Shader compiliation failed." << std::endl
					<< "  Cause:  " << buffer << std::endl;

				return false;
			}

			shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(shader,1,&fragment_shader_source,nullptr);
			glCompileShader(shader);
			glGetShaderiv(shader,GL_COMPILE_STATUS,&shader_status);
			if(shader_status == GL_TRUE)
			{
				glAttachShader(program,shader);
			}
			else
			{
				char buffer[2048];


				glGetShaderInfoLog(shader,2048,nullptr,buffer);
				std::clog
					<< "[Error]" << std::endl
					<< "  Shader compiliation failed." << std::endl
					<< "  Cause:  " << buffer << std::endl;

				return false;
			}
		}

		glBindFragDataLocation(program,0,"outColor");
		glLinkProgram(program);
		glUseProgram(program);

		GLint position_attribute;
		{
			position_attribute = glGetAttribLocation(program,"position");
			glEnableVertexAttribArray(position_attribute);
			glVertexAttribPointer(position_attribute,2,GL_FLOAT,GL_FALSE,4 * sizeof(float),0);
		}

		GLint vertexuv_attribute;
		{
			vertexuv_attribute = glGetAttribLocation(program,"vertex_uv");
			glEnableVertexAttribArray(vertexuv_attribute);
			glVertexAttribPointer(vertexuv_attribute,2,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void*)(2 * sizeof(float)));
		}
	}

	return true;
}

bool GLWindow::initPixelFormat()
{
	int                   pixel_format;
	PIXELFORMATDESCRIPTOR pixel_format_descriptor;


	memset(&pixel_format_descriptor,0,sizeof(pixel_format_descriptor));
	pixel_format_descriptor.nSize = sizeof(pixel_format_descriptor);
	pixel_format_descriptor.nVersion = 1;
	pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION;
	pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
	pixel_format_descriptor.cColorBits = 32;
	pixel_format_descriptor.cDepthBits = 24;
	pixel_format_descriptor.cRedBits = 8;
	pixel_format_descriptor.cGreenBits = 8;
	pixel_format_descriptor.cBlueBits = 8;
	pixel_format_descriptor.cAlphaBits = 8;
	pixel_format_descriptor.cStencilBits = 8;
	pixel_format_descriptor.iLayerType = PFD_MAIN_PLANE;

	pixel_format = ChoosePixelFormat(this->device_context_handle,&pixel_format_descriptor);
	if(pixel_format == 0)
	{
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to update pixel format." << std::endl
			<< "  Cause:  " << "No suitable pixel formats available." << std::endl;

		return false;
	}

	if(!SetPixelFormat(this->device_context_handle,pixel_format,&pixel_format_descriptor))
	{
		LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to update pixel format." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?

		return false;
	}

	return true;
}

bool GLWindow::initRenderContext()
{
	this->render_context_handle = wglCreateContext(this->device_context_handle);
	if(this->render_context_handle == nullptr)
	{
		LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Could not retrieve a render context." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?

		return false;
	}

	/* Activate the rendering context. */
	if(!wglMakeCurrent(this->device_context_handle,this->render_context_handle))
	{
		LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to activate render context." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?

		return false;
	}

	return true;
}

void GLWindow::onDraw(Surface& surface)
{

}

void GLWindow::onIdle()
{
	this->draw(*this->surface);
	this->surface->flush();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->surface->bind();
	glDrawArrays(GL_QUADS,0,4);
	this->surface->unbind();

	SwapBuffers(this->device_context_handle);
}

bool GLWindow::onInit()
{
	if(!this->initDeviceContext())
	{
		this->destroy();
		return false;
	}

	if(!this->initPixelFormat())
	{
		this->destroy();
		return false;
	}

	if(!this->initRenderContext())
	{
		this->destroy();
		return false;
	}

	if(!this->initGL())
	{
		this->destroy();
		return false;
	}

	/* Display the window. */
	this->enableTransparency();
	
	ShowWindow(*this,SW_SHOW);
	SetForegroundWindow(*this);
	SetFocus(*this);

	return true;
}

void GLWindow::onResize(unsigned width,unsigned height)
{
	this->Window::onResize(width,height);

	if(!this->isInitialized())
	{
		return;
	}

	this->surface->resize(width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,width,height);
	gluPerspective(45.0f,(GLfloat)width / (GLfloat)(height == 0 ? 1 : height),0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}