
#ifndef _TNA_DIRECTORY_REGISTRY_H_
#define _TNA_DIRECTORY_REGISTRY_H_ value

#include "../types.h"

namespace tna 
{

void
directory_registry_init();

void
directory_registry_release();
  
/**
 * @brief Registers a directory to be searched for resources
 *
 * @param directory The directory to register
 */
void 
directory_registry_insert(const char* directory);

/**
 * @brief Gets the path to a resource
 *
 * @param resource_name The name of the resource
 *
 * @return 
 */
uint32_t
directory_registry_get_path(const char* resource_name,
                            char* buffer,
                            uint32_t buffer_length);

} /* resources */ 
#endif /* ifndef _TNA_DIRECTORY_REGISTRY_H_ */

