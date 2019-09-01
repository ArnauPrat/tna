

#ifndef _TNA_VERTEX_H_
#define _TNA_VERTEX_H_ value

#include "../math/vector.h"

namespace tna 
{

struct Vertex 
{
  vector3_t m_position;
  vector3_t m_color;
  vector2_t m_tex_coord;

  bool operator==(const Vertex& other) const 
  {
    return m_position == other.m_position && m_color == other.m_color && m_tex_coord == other.m_tex_coord;
  }

};
  
} /* tna */ 

#endif /* ifndef _TNA_VERTEX_H_ */

