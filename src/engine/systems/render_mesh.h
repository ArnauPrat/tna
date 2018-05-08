

#ifndef _TNA_RENDER_MESH_H_
#define _TNA_RENDER_MESH_H_ value

#include "../engine.h"
#include <iostream>


namespace tna {

class RenderMesh {
public:

  void run(furious::Context* context, 
           int32_t id, 
           const Mesh* mesh, 
           const Transform* transform) {

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), transform->m_position);
    glm::mat4 rotateX = glm::rotate(translate, transform->m_rotation.x, glm::vec3{1.0, 0.0, 0.0});
    glm::mat4 rotateY = glm::rotate(rotateX, transform->m_rotation.y, glm::vec3{0.0, 1.0, 0.0});
    glm::mat4 rotateZ = glm::rotate(rotateY, transform->m_rotation.z, glm::vec3{0.0, 0.0, 1.0});
    glm::mat4 model = glm::scale(rotateZ, transform->m_scale);

    rendering::render_mesh(mesh->m_mesh_data, model);

  }

};
} /* tna */ 
#endif /* ifndef _TNA_RENDER_MESH_H_ */


