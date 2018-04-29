

#ifndef _TNA_VERTEX_H_
#define _TNA_VERTEX_H_ value

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>


namespace tna {
namespace rendering {

struct Vertex {
  glm::vec2 m_position;
  //glm::vec2 m_tex_coord;
  glm::vec3 m_color;

  static VkVertexInputBindingDescription get_binding_description() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {

    std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions = {};

    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[0].offset = offsetof(Vertex, m_position);

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = offsetof(Vertex, m_color);

    return attribute_descriptions;
  }

};
  
}
} /* tna */ 

#endif /* ifndef _TNA_VERTEX_H_ */

