

#include "directory_registry.h"

#include <iostream>

namespace tna
{

template<typename T>
TnaResourceRegistry<T>::~TnaResourceRegistry<T>() 
{
  for(auto pair : m_resources) {
    T::unload(pair.second);
  }
  m_resources.clear();
}


template<typename T>
T* TnaResourceRegistry<T>::load(const std::string& resource_name) 
{
  auto it = m_resources.find(resource_name);
  if(it == m_resources.end()) {
    std::string path = get_path(resource_name);
    if(!path.empty()) {
      T* resource = T::load(path);
      m_resources.insert(std::make_pair(resource_name, resource));
      return resource;
    }
    return nullptr;
  } else 
  {
    return it->second;
  }
}

template<typename T>
void  TnaResourceRegistry<T>::unload(const std::string& resource_name) 
{
  auto it = m_resources.find(resource_name);
  if(it != m_resources.end()) 
  {
    T::unload(it->second);
    m_resources.erase(it);
  }
}

template<typename T>
void  TnaResourceRegistry<T>::clear() 
{
  for(auto pair : m_resources) {
    T::unload(pair.second);
  }
  m_resources.clear();
}

} /* tna */ 
