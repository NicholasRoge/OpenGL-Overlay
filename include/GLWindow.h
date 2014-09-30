#ifndef GLWindow_H
#define GLWindow_H

#include <map>
#include <Strixa/Graphics/GLSurface.h>
#include <Strixa/UI/Window.h>
#include <Windows.h>


class GLWindow : public Strixa::UI::Window
{
	/* Member Methods */
	private:
		HDC                                          device_context_handle;
		HGLRC                                        render_context_handle;
		std::unique_ptr<Strixa::Graphics::GLSurface> surface;

	/* Member Properties */
	private:
		bool initDeviceContext();

		bool initGL();

		bool initPixelFormat();

		bool initRenderContext();

		bool initWindow();

		virtual void onDraw(Strixa::Graphics::Surface& surface);

	protected:
		virtual void destroy();

		virtual LRESULT handleMessage(UINT message,WPARAM w_param,LPARAM l_param);

		virtual WNDCLASSEX getWindowClass() const;

		virtual void onIdle();

		virtual bool onInit();

		virtual void onResize(unsigned previous_width,unsigned previous_height);

	public:
		GLWindow();
};

#endif