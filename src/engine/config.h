
#ifndef _TNA_CONFIG_H_
#define _TNA_CONFIG_H_ value

#include "common.h"
#include <string>

namespace tna 
{

/**
 * @brief Structure used to store the engine's configuration
 */
struct TnaConfig
{

  TnaConfig();
  ~TnaConfig();

  /**
   * \brief Inserts a vulkan validation layer to the configuration
   *
   * \param layer_name The name of the layer
   */
  void
  insert_vk_layer(const std::string& layer_name); 

  uint32_t        m_viewport_width  = 1440;
  uint32_t        m_viewport_height = 900;
  bool            m_fullscreen    = false;

  uint32_t        m_max_vk_vlayers;
  uint32_t        m_num_vk_vlayers;
  std::string**   m_vk_vlayers;

};


/**
 * \brief Loads the configuration stored in the given configuration file
 *
 * \param configFileName The file containing the configuration
 * \param config A pointer to the config object to store the configuration to
 *
 * \return E_NO_ERROR if successful.  
 */
void 
load_config(const std::string& file_name, 
            TnaConfig* config);

}

#endif /* ifndef _TNA_CONFIG_H_ */
