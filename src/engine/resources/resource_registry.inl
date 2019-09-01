

#include "directory_registry.h"

namespace tna
{

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
TnaResourceRegistry<T,CreateFunc,DestroyFunc>::~TnaResourceRegistry<T,CreateFunc,DestroyFunc>() 
{
  for(auto pair : m_resources) {
    DestroyFunc(pair.second);
  }
  m_resources.clear();
}


template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
T* TnaResourceRegistry<T,CreateFunc,DestroyFunc>::load(const std::string& resource_name) 
{
  auto it = m_resources.find(resource_name);
  if(it == m_resources.end()) 
  {
    std::string path = get_path(resource_name);
    if(!path.empty()) 
    {
      T* resource = CreateFunc(path.c_str());
      m_resources.insert(std::make_pair(resource_name, resource));
      return resource;
    }
    return nullptr;
  } 
  else 
  {
    return it->second;
  }
}

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
void  TnaResourceRegistry<T,CreateFunc,DestroyFunc>::unload(const std::string& resource_name) 
{
  auto it = m_resources.find(resource_name);
  if(it != m_resources.end()) 
  {
    DestroyFunc(it->second);
    m_resources.erase(it);
  }
}

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
void  TnaResourceRegistry<T,CreateFunc,DestroyFunc>::clear() 
{
  for(auto pair : m_resources) {
    DestroyFunc(pair.second);
  }
  m_resources.clear();
}

} /* tna */ 
