#ifndef Strixa_UI_Button_H
#define Strixa_UI_Button_H

#include <Strixa/UI/Pane.h>


namespace Strixa
{
	namespace UI
	{
		class Button : public Pane
		{
			/* Member Properties */
			private:
				std::string text;
				
			/* Member Methods */
			private:
				virtual void onDraw(Graphics::Surface& surface);
				
			public:
				Button(String text);
				
				const std::string& getText() const;
				
				void setText(const std::string& text);
		};
	}
}

#endif