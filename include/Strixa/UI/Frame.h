#ifndef Strixa_UI_Frame_H
#define Strixa_UI_Frame_H

#include <Strixa/UI/Pane.h>
#include <vector>


namespace Strixa
{
	namespace UI
	{
		class Frame : public Pane
		{
			friend class Pane;

			/* Member Properties */
			private:
				std::vector<Pane*> children;
            
			/* Member Methods */
			protected:
				virtual void draw(Graphics::Surface& surface);
				
				virtual void onChildMoveRequest(Pane& child,int x,int y);

				virtual void onChildResizeRequest(Pane& child,unsigned x,unsigned y);

			public:
				Frame();

				template<typename PaneType>
				size_t addChild(PaneType& pane);

				template<typename PaneType>
				size_t addChild(PaneType&& pane);

				template<typename PaneType,typename... ConstructorArguments>
				size_t addChild(ConstructorArguments&&... arguments);

				void removeChild(size_t cpo);

				/*LayoutManager& getLayoutManager() const;

				void setLayoutManager(const LayoutManager& manager);*/

				Pane& operator [](size_t cpo);
		};

		template<typename PaneType>
		size_t Frame::addChild(PaneType& pane)
		{
			return this->addChild(std::move(pane));
		}

		template<typename PaneType>
		size_t Frame::addChild(PaneType&& pane)
		{
			static_assert(std::is_base_of<Pane,PaneType>::value);

			this->children.push_back(new PaneType(std::move(pane)));
			return this->children.size() - 1;
		}

		template<typename PaneType,typename... ConstructorArguments>
		size_t Frame::addChild(ConstructorArguments&&... arguments)
		{
			static_assert(std::is_base_of<Pane,PaneType>::value,"Herp");

			Pane* child;


			child = new PaneType(std::forward(arguments)...);
			child->parent = this;
			this->children.push_back(child);

			return this->children.size() - 1;
		}
	}
}

#endif