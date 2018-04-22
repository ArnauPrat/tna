
#ifndef _TNA_CONFIG_H_
#define _TNA_CONFIG_H_ value

#include "types.h"
#include <string>


namespace tna {

/**
 * @brief Structure used to store the engine's configuration
 */
struct Config {

  int32_t   m_viewport_width  = 1440;
  int32_t   m_viewport_height = 900;
  bool      m_fullscreen    = false;

};


/**
 * @brief Loads the configuration stored in the given configuration file
 *
 * @param configFileName The file containing the configuration
 *
 * @return Returns a Config object with the configuration specified in the input
 * file
 */
Config load_config(const std::string& configFileName);

}

#endif /* ifndef _TNA_CONFIG_H_ */
