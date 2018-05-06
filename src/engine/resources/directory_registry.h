
#ifndef _TNA_DIRECTORY_REGISTRY_H_
#define _TNA_DIRECTORY_REGISTRY_H_ value

#include <vector>
#include "../optional.h"

namespace tna {
namespace resources {
  
/**
 * @brief Registers a directory to be searched for resources
 *
 * @param directory The directory to register
 */
void register_directory(const std::string& directory);

/**
 * @brief Gets the path to a resource
 *
 * @param resource_name The name of the resource
 *
 * @return 
 */
optional<std::string> get_path(const std::string& resource_name);

} /* tna */ 
} /* resources */ 
#endif /* ifndef _TNA_DIRECTORY_REGISTRY_H_ */

