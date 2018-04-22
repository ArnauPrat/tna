
#include "common.h"
#include "config.h"
#include <fstream>
#include <sstream>

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
      log->print("Parsed option %s with value %s", option.c_str(), value.c_str());
    }
    inFile.close();
  } else {
  }
  return config;
}
}
