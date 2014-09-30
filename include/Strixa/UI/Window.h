#ifndef Strixa_UI_Window_H
#define Strixa_UI_Window_H

#include <map>
#include <Strixa/UI/Frame.h>
#include <Windows.h>


namespace Strixa
{
	namespace UI
	{
		class Window : public Strixa::UI::Frame
		{
			/* Class Properties */
			private:
				static std::map<HWND,Window*> window_by_hwnd;

			/* Class Methods */
			private:
				static LRESULT CALLBACK delegateMessage(HWND window_handle,UINT message,WPARAM w_param,LPARAM l_param);

			/* Member Properties */
			private:
				bool       base_handler_called;
				bool       initialized;
				WNDCLASSEX window_class;
				HWND       window_handle;

			/* Member Methods */
			private:
				bool initWindow();

				bool registerWindowClass();

			protected:
				virtual void destroy();

				virtual WNDCLASSEX getWindowClass() const = 0;

				virtual LRESULT handleMessage(UINT message,WPARAM w_param,LPARAM l_param);

				virtual void onIdle();

				virtual bool onInit();

				virtual void onMove(int x,int y);

				virtual void onResize(unsigned width,unsigned height);

			public:
				Window();

				virtual ~Window();

				void disableTransparency();

				void enableTransparency();

				virtual bool init();

				bool isInitialized() const;

				void maximize();

				int operator ()();

				operator HWND() const;
		};
	}
}

#endif