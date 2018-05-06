
#include "common.h"
#include "config.h"
#include <fstream>
#include <sstream>
#include "resources/directory_registry.h"

namespace tna {
Config load_config(const std::string& configFileName) {

  Config config;
  std::ifstream inFile;
  inFile.open(configFileName);
  if (inFile) {
    std::string option;
    std::string value;
    while (inFile >> option) {
      if (option == "ViewportWidth") {
        inFile >> value;
        std::stringstream ss(value);
        ss >> config.m_viewport_width;
      }

      if (option == "ViewportHeight") {
        inFile >> value;
        std::stringstream ss(value);
        ss >> config.m_viewport_height;
      }

      if (option == "FullScreen") {
        inFile >> value;
        std::stringstream ss(value);
        ss >> config.m_fullscreen;
      }

      if (option == "VkValidationLayer") {
        inFile >> value;
        std::stringstream ss(value);
        config.m_vk_validation_layers.push_back(ss.str());
      }

      if (option == "ResourceDirectory") {
        inFile >> value;
        resources::register_directory(value); 
      }

      log->log("Parsed option %s with value %s", option.c_str(), value.c_str());
    }
    inFile.close();
  } else {
  }
  return config;
}
}
