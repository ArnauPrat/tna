


#ifndef _TNA_RESOURCE_REGISTRY_H_
#define _TNA_RESOURCE_REGISTRY_H_ value

#include <string>
#include <map>

namespace tna 
{

template<typename T, T*(*CreateFunc)(const char*), void(*DestroyFunc)(T*)>
class TnaResourceRegistry 
{
public:
  TnaResourceRegistry() = default;
  TnaResourceRegistry(TnaResourceRegistry&) = delete;

  virtual ~TnaResourceRegistry();

  void operator=(TnaResourceRegistry&) = delete;

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////

  /**
   * \brief Loads a resource 
   *
   * \param resource_name The name of the resource to load
   *
   * \return Returns an optional containing the ponter to the resource if it
   * was successfully loaded.
   */
  T* 
  load(const char* resource_name);

  /**
   * \brief Unloads a resource
   *
   * \param resource_name The name of the resource to unload
   */
  void  
  unload(const char* resource_name);


  /**
   * \brief Clears all resources
   */
  void 
  clear();
private:

  std::map<char*, T*> m_resources; 

};

} /* tna */ 

#include "resource_registry.inl"

#endif /* ifndef _TNA_RESOURCE_REGISTRY_H_ */
