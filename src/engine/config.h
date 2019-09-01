
#ifndef _TNA_CONFIG_H_
#define _TNA_CONFIG_H_ value

#include "common.h"

namespace tna 
{

#define _TNA_MAX_VK_VALIDATION_LAYERS 128 

/**
 * @brief Structure used to store the engine's configuration
 */
struct config_t 
{


  uint32_t        m_viewport_width  = 1440;
  uint32_t        m_viewport_height = 900;
  bool            m_fullscreen      = false;

  uint32_t        m_num_vk_vlayers  = 0;
  char*           m_vk_vlayers[_TNA_MAX_VK_VALIDATION_LAYERS];

};


/**
 * \brief Loads the configuration stored in the given configuration file
 *
 * \param config A pointer to the config object to store the configuration to
 * \param configFileName The file containing the configuration
 *
 * \return E_NO_ERROR if successful.  
 */
void 
config_init(config_t* config,
            const char* file_name);

/**
 * \brief Releases the configuration object
 *
 * \param config
 */
void
config_release(config_t* config);

/**
 * \brief Inserts a vulkan validation layer to the configuration
 *
 * \param layer_name The name of the layer
 */
void
config_insert_vk_layer(config_t* config,
                       const char* layer_name); 

}

#endif /* ifndef _TNA_CONFIG_H_ */
