


#include <iostream>

namespace tna {
namespace resources {
  

template<typename T>
  ResourceRegistry<T>::~ResourceRegistry<T>() {
    for(auto pair : m_resources) {
      T::unload(pair.second);
    }
    m_resources.clear();
  }


template<typename T>
  optional<T*> ResourceRegistry<T>::load(const std::string& resource_name) {
    auto it = m_resources.find(resource_name);
    if(it == m_resources.end()) {
      auto path = get_path(resource_name);
      if(path) {
        T* resource = T::load(*path);
        m_resources.insert(std::make_pair(resource_name, resource));
        return optional<T*>{resource};
      }
      return optional<T*>{};
    } else {
      return optional<T*>{it->second};
    }
  }

template<typename T>
  void  ResourceRegistry<T>::unload(const std::string& resource_name) {
    auto it = m_resources.find(resource_name);
    if(it != m_resources.end()) {
      T::unload(it->second);
      m_resources.erase(it);
    }
  }

template<typename T>
  void  ResourceRegistry<T>::clear() {
    for(auto pair : m_resources) {
      T::unload(pair.second);
    }
    m_resources.clear();
  }

} /* resources */ 
} /* tna */ 
