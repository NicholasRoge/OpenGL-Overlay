#include <fstream>
#include <GLWindow.h>
#include <iostream>
#include <Windows.h>
#include <Strixa/UI/Pane.h>

using Strixa::UI::Pane;


/* Main Method */
int CALLBACK WinMain(HINSTANCE current_instance_handle,HINSTANCE previous_instance_handle,LPSTR arguments,int show_command)
{
	std::ofstream log;
	GLWindow      window;
	

	log.open("./Debug/output.log",std::ofstream::ate);
	std::cout.rdbuf(log.rdbuf());

	
	/* Initialize the window and run it's message loop. */
	if(!window.init())
	{
		return 1;
	}

	window.setBackgroundColour(Strixa::Graphics::Colour(0,0,0,100));
	window.maximize();

    return window();
}