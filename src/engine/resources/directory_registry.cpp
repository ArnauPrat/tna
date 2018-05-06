
#include "directory_registry.h"
#include <vector>
#include <fstream>
#include <iostream>

namespace tna {
namespace resources {
  

static std::vector<std::string> directories;

void register_directory(const std::string& directory) {
  directories.push_back(directory);
}

optional<std::string> get_path(const std::string& resource_name) {

  for(auto directory : directories) {
    std::string path = directory+"/"+resource_name; 
    std::ifstream ifile( path );
    if (ifile) { 
      return optional<std::string>{path}; 
    }
  } 

  return optional<std::string>{};

}
  
}
} /* tna */ 
