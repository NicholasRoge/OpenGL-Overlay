template<typename ManagedResourceType>
class ResourceManager
{
	public:
		class Resource
		{
			friend class ResourceManager;

			private:
				unsigned identifier;
				unsigned handles_out;

			private:
				Resource(unsigned identifier)
				{
					this->identifier = identifier;
					this->handles_out = 0;
				}

			public:
				Resource() = delete;

				Resource(const Resource& copy) = delete;

				Resource(const Resource&& move) = delete;

				~Resource()
				{
					if(this->handles_out == 1)
					{
						//Because the 
						delete this;
					}
				}

				template<typename ResourceType>
				ResourceType& as() const
				{
					return *((ResourceType*)this);
				}

				unsigned long long getID() const
				{
					return this->identifier;
				}

				bool isValid() const
				{
					return this->valid;
				}
		};

	private:
		std::vector<Resource> handles;
		std::stack<unsigned>  free_ids;

	private:
		unsigned getNewIdentifier()
		{
			if(this->free_ids.size() == 0)
			{
				unsigned identifier;


				identifier = this->free_ids.top();
				this->free_ids.pop();

				return identifier;
			}
			else
			{
				this->handles.insert(nullptr);

				return handles.size() - 1;
			}
		}

	public:
		template<typename ResourceType>
		unsigned create()
		{
			Resource* resource;


			resource = new ResourceType(this->getNewIdentifier());
		}

		void free(unsigned id)
		{

		}

		ManagedResourceType& getFromId(unsigned id)
		{
			if(id >= this->handles.size() || this->handles[id] == nullptr)
			{
				throw std::exception();
			}

			return *(this->handles[id]);
		}
};

template<typename ManagedResourceType>
class ResourceManager<typename std::enable_if<!std::is_base_of<Resource,ManagedResourceType>::value>::type>{
	#error Wrong class bro.  The templated type has to be a child of the Resource type.
};

template<typename ResourceType>
typename std::enable_if<!std::is_base_of<ManagedResourceType,ResourceType>::value>::type* ResourceManager<ResourceType>::create()
{
	#error Oh I dun fucked shit up.
}

template<typename ManagedResourceType,bool is_default_constructable>
template<>
class ResourceManager<ManagedResourceType,is_default_constructable>::AbstractResource<true>
{
	public:
		AbstractResource()
		{
			this->identifier = ResourceManager<ManagedResourceType,is_default_constructable>::global_manager.getNewIdentifier();
		}
};