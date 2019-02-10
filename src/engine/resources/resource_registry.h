


#ifndef _TNA_RESOURCE_REGISTRY_H_
#define _TNA_RESOURCE_REGISTRY_H_ value

#include <string>
#include <map>

namespace tna 
{

template<typename T>
class ResourceRegistry 
{
public:
  ResourceRegistry() = default;
  ResourceRegistry(ResourceRegistry&) = delete;

  virtual ~ResourceRegistry();

  void operator=(ResourceRegistry&) = delete;

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
  load(const std::string& resource_name);

  /**
   * \brief Unloads a resource
   *
   * \param resource_name The name of the resource to unload
   */
  void  
  unload(const std::string& resource_name);


  /**
   * \brief Clears all resources
   */
  void 
  clear();
private:

  std::map<std::string, T*> m_resources; 

};

} /* tna */ 

#include "resource_registry.inl"

#endif /* ifndef _TNA_RESOURCE_REGISTRY_H_ */
