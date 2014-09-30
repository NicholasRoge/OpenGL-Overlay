#ifndef SYSTEM_RESOURCE_H
#define SYSTEM_RESOURCE_H

#include <iostream>
#include <stack>
#include <vector>

typedef size_t resid;

namespace System
{
	template<typename ResourceType>
	class ResourceManager
	{
		private:
			std::vector<ResourceType*> handles;
			std::stack<resid>          unused_resids;

		public:
			virtual ~ResourceManager()
			{
				for(resid resource_id = 0;resource_id < this->handles.size();++resource_id)
				{
					if(this->handles[resource_id] != nullptr)
					{
						std::clog << "Warning:  Resource with resource_id=" << resource_id << " was not freed before it's manager was destroyed." << std::endl;
						this->free(resource_id);
					}
				}
			}

			template<typename DerivedResourceType = ResourceType,typename... CArgs>
			resid aquire(CArgs... cargs)
			{
				ResourceType* resource;
				
				
				resource = new ResourceType(std::forward(cargs)...);
				//TODO:  Critical section.  unused_resids and handles must not be manipulated by any other threads.
				if(this->unused_resids.size() > 0)
				{
					resource->resource_id = this->unused_resids.top();
					this->unused_resids.pop();
					this->handles[resource->resource_id] = resource;
				}
				else
				{
					this->handles.push_back(resource);
					resource->resource_id = this->handles.size() - 1;
				}
				//TODO:  End critical section.
				
				return resource->resource_id;
			}

			void free(resid resource_id)
			{
				//TODO:  Critical section.  free_resids and handles must not be manipulated by any other threads.
				if(resource_id >= this->handles.size() || this->handles[resource_id] != nullptr)
				{
					//Fail silently
					return;
				}
								
				delete this->handles[resource_id];
				this->handles[resource_id] = nullptr;
				
				this->unused_resids.push(resource_id);
				//TODO:  End critical section.
			}

			ResourceType& operator [](resid resource_id)
			{
				//TODO:  Critical section.  handles must not be manipulated by any other threads.
				ResourceType* resource;
				
				
				if(resource_id >= this->handles.size() || this->handles[resource_id] != nullptr)
				{
					throw std::exception();
				}
				
				resource = this->handles[resource_id];
				//TODO:  End critical section.				

				return *resource;
			}
	};

/**
			 * Abstract class which represents some kind of resource.
 			 *
			 * Note:  Implementers of this class form a contract with the Resource class to only be instantiated using ResourceManager<TYPE>::aquire(...).
			 */
			class Resource
			{
				template<typename ResourceType>
				friend class ResourceManager;

				/* Member Properties */
				private:
					resid resource_id;
					
				/* Member Methods */	
				public:
					resid getResourceID() const
					{
						return this->resource_id;
					}
				};
}

#endif