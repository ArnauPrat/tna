



namespace tna {

template<typename T>
  ResourceRegistry<T>::~ResourceRegistry<T>() {
    for(auto pair : m_resources) {
      T::unload(pair.second);
    }
  }


template<typename T>
  optional<T*> ResourceRegistry<T>::load(const std::string& resource_name) {
    auto path = get_path(resource_name);
    if(path) {
      T* resource = T::load(*path);
      return optional<T*>{resource};
    }
    return optional<T*>{};
  }

template<typename T>
  void  ResourceRegistry<T>::unload(const std::string& resource_name) {
    auto it = m_resources.find(resource_name);
    if(it != m_resources.end()) {
      T::unload(it.second);
    }
  }

} /* tna */ 
