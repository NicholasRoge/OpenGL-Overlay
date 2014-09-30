#ifndef UI_PaneManager_H
#define UI_PaneManager_H

#include <UI/Pane.h>

namespace UI
{
    class PaneManager
    {
        public:
            template<typename PaneType,typename... ConstructorArguments>
            typename std::enable_if<std::is_base_of<Pane,PaneType>::value && std::is_constructible<PaneType,ConstructorArguments...>::value>::type& addChild(ConstructorArguments... constructor_arguments)
            {
                
            }
    }
}

#endif