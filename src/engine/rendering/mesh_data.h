

#ifndef _TNA_MESHDATA_H_
#define _TNA_MESHDATA_H_ value

#include "vertex.h"
#include "../types.h"
#include "vulkan/vulkan.h"

#include <string>


namespace tna {
namespace rendering {
  
struct MeshData {

  MeshData() = default;
  virtual ~MeshData() = default;

  static MeshData* load(const std::string& path);

  static void unload(MeshData* shader);
};
  
} /* rendering */ 
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
