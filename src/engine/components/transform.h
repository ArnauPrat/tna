

#ifndef _TNA_TRANSFORM_H_
#define _TNA_TRANSFORM_H_ value

#include "../engine.h"

namespace tna {

struct Transform {

  Transform() : 
    m_position{0.0f, 0.0f, 0.0f},
    m_rotation{0.0f, 0.0f, 0.0f},
    m_scale{1.0f, 1.0f, 1.0f}
  {

  }

  Transform(const glm::vec3& position) :
    m_position{position},
    m_rotation{0.0f, 0.0f, 0.0f},
    m_scale{1.0f, 1.0f, 1.0f}
  {
  }

  Transform(const glm::vec3& position, const glm::vec3& rotation) : 
    m_position{position},
    m_rotation{rotation},
    m_scale{1.0f, 1.0f, 1.0f}
  {
  }

  Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
    m_position{position},
    m_rotation{rotation},
    m_scale{scale}
  {
  }

  static std::string name() {
    return "Transform";
  }

  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;


};
  
} /* tna */ 
#endif /* ifndef _TNA_TRANSFORM_H_ */
