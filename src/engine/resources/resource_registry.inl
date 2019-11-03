

#include "directory_registry.h"
#include "../log.h"
#include <string.h>

namespace tna
{

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
TnaResourceRegistry<T,CreateFunc,DestroyFunc>::~TnaResourceRegistry<T,CreateFunc,DestroyFunc>() 
{
  /*for(auto pair : m_resources) {
    delete [] pair.first;
    DestroyFunc(pair.second);
  }
  m_resources.clear();
  */
  clear();
}


template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
T* TnaResourceRegistry<T,CreateFunc,DestroyFunc>::load(const char* resource_name) 
{
  for(auto pair : m_resources) 
  {
    if(strcmp(pair.first, resource_name) == 0 ) 
    {
      return pair.second;
    }
  }

  char* path = new char[2048];
  uint32_t length = directory_registry_get_path(resource_name,
                                                path,
                                                2048);
  if(length >= 2048)
  {
    TNA_LOG_ERROR(TNA_ERROR::E_IO_UNEXPECTED_INPUT_ERROR, 
                  "Path buffer in registry is not large enough");
  }

  T* resource = nullptr;
  if(strlen(path) > 0) 
  {
    resource = CreateFunc(path);
    char* tmp = new char[strlen(resource_name)+1];
    strcpy(tmp, resource_name);
    m_resources.insert(std::make_pair(tmp, resource));
  }

  delete [] path;
  return resource;
}

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
void  TnaResourceRegistry<T,CreateFunc,DestroyFunc>::unload(const char* resource_name) 
{
  for(auto pair : m_resources) 
  {
    if(strcmp(pair.first, resource_name) == 0 ) 
    {
      char* name = pair.first;
      T* resource = pair.second;
      m_resources.erase(m_resources.find(pair.first));
      delete [] name;
      DestroyFunc(resource);
      return;
    }
  }
}

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
void  TnaResourceRegistry<T,CreateFunc,DestroyFunc>::clear() 
{
  for(auto pair : m_resources) 
  {
    delete [] pair.first;
    DestroyFunc(pair.second);
  }
  m_resources.clear();
}

} /* tna */ 
