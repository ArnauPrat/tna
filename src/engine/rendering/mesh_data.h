

#ifndef _TNA_MESHDATA_H_
#define _TNA_MESHDATA_H_ value


#include "../common.h"
#include <string>

namespace tna 
{
  
struct MeshData 
{
  static MeshData* 
  load(const std::string& path);

  static void 
  unload(MeshData* mesh_data);
};
  
} /* tna */ 

#endif /* ifndef _TNA_MESH_H_ */
