#pragma comment(lib,"dwmapi.lib")

#include <Strixa/UI/Window.h>

#include <dwmapi.h>

using Strixa::UI::Window;


/* Class [Strixa::UI::Window] Definition */
std::map<HWND,Window*> Window::window_by_hwnd;


Window::Window()
{
	this->initialized = false;
	this->base_handler_called = false;
	this->window_handle = nullptr;
}

Window::~Window()
{
}

LRESULT CALLBACK Window::delegateMessage(HWND window_handle,UINT message,WPARAM w_param,LPARAM l_param)
{
	if(message == WM_CREATE)
	{
		Window::window_by_hwnd[window_handle] = (Window*)((CREATESTRUCT*)l_param)->lpCreateParams;
	}
	else if(Window::window_by_hwnd[window_handle] == nullptr)
	{
		return DefWindowProc(window_handle,message,w_param,l_param);
	}

	return Window::window_by_hwnd[window_handle]->handleMessage(message,w_param,l_param);
}

void Window::destroy()
{
	if(this->window_handle != nullptr && !DestroyWindow(this->window_handle))
	{
		/*LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to destroy window." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/
	}
	this->window_handle = nullptr;

	this->initialized = false;
}

void Window::disableTransparency()
{
	DWM_BLURBEHIND blurbehind;


	memset(&blurbehind,0,sizeof(blurbehind));
	blurbehind.fEnable = false;
	DwmEnableBlurBehindWindow(this->window_handle,&blurbehind);
}

void Window::enableTransparency()
{
	DWM_BLURBEHIND blurbehind;


	memset(&blurbehind,0,sizeof(blurbehind));
	blurbehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	blurbehind.fEnable = true;
	blurbehind.hRgnBlur = CreateRectRgn(0,0,-1,-1);
	DwmEnableBlurBehindWindow(this->window_handle,&blurbehind);
}

LRESULT Window::handleMessage(UINT message,WPARAM w_param,LPARAM l_param)
{
	switch(message)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);

			return 0;
		}

		case WM_CREATE:
		{
			this->base_handler_called = true;

			break;
		}
		
		case WM_PAINT:
		{
			//this->draw(this->surface);  //TODO:  Some kind of surface which encompasses the HDC Windows style drawing.
		
			break;
		}

		/*case WM_SIZE:
		{
			this->onResize(LOWORD(l_param),HIWORD(l_param));

			return 0;
		}*/

		/*case WM_SYSCOMMAND:
		{
			switch(w_param)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}

			break;
		}*/
	}

	return DefWindowProc(window_handle,message,w_param,l_param);
}

bool Window::init()
{
	if(!this->registerWindowClass())
	{
		return false;
	}

	if(!this->initWindow())
	{
		return false;
	}

	if(!this->onInit())
	{
		return false;
	}

	this->initialized = true;

	return true;
}

bool Window::initWindow()
{
	this->window_handle = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		this->window_class.lpszClassName,
		nullptr,
		WS_POPUP,
		this->getX(),
		this->getY(),
		this->getWidth(),
		this->getHeight(),
		NULL,
		NULL,
		this->window_class.hInstance,
		(void*)this
	);  //TODO:  This needs to be looked at as this won't be teh same across all classes
	if(this->window_handle == nullptr || !this->base_handler_called)
	{
		/*LPTSTR error_message;


		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
		std::clog
			<< "[Error]" << std::endl
			<< "  Unable to retrieve a window handle." << std::endl
			<< "  Cause:  " << error_message << std::endl;
		LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/

		return false;
	}

	return true;
}

bool Window::isInitialized() const
{
	return this->initialized;
}

bool Window::registerWindowClass()
{
	this->window_class = this->getWindowClass();
	if(!GetClassInfoEx(this->window_class.hInstance,this->window_class.lpszClassName,nullptr))
	{
		this->window_class.lpfnWndProc = window_class.lpfnWndProc = Window::delegateMessage;

		if(!RegisterClassEx(&this->window_class))
		{
			/*LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			std::clog
			<< "[Error]" << std::endl
			<< "  Unable to register window class." << std::endl
			<< "  Cause:  " << error_message << std::endl;
			LocalFree((LPTSTR)&error_message);  //TODO:  Is this right?*/

			return false;
		}
	}

	return true;
}

void Window::maximize()
{
	this->resize(GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION));
}

void Window::onIdle()
{
}

bool Window::onInit()
{
	return true;
}

void Window::onMove(int x,int y)
{
	if(this->isInitialized())
	{
		SetWindowPos(this->window_handle,nullptr,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}
	this->Frame::onMove(x,y);
}

void Window::onResize(unsigned width,unsigned height)
{
	if(this->isInitialized())
	{
		SetWindowPos(this->window_handle,nullptr,0,0,width,height,SWP_NOMOVE | SWP_NOZORDER);
	}
	this->Frame::onResize(width,height);
}

int Window::operator ()()
{
	MSG  message;
	bool running;


	running = true;
	while(running)
	{
		if(PeekMessage(&message,this->window_handle,0,0,PM_REMOVE))
		{
			if(message.message == WM_QUIT)
			{
				running = false;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			this->onIdle();
		}
	}
	this->destroy();

	return message.wParam;  //message.wParam contains the exit code sent to PostQuitMessage.
}

Window::operator HWND() const
{
	return this->window_handle;
}