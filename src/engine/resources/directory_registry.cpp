
#include "directory_registry.h"
#include <vector>
#include <stdio.h>

namespace tna 
{

static std::vector<std::string> directories;

void 
register_directory(const std::string& directory) 
{
  directories.push_back(directory);
}

std::string 
get_path(const std::string& resource_name) 
{
  for(std::string& directory : directories) 
  {
    std::string path = directory+"/"+resource_name; 
    FILE* fd = fopen(path.c_str(),"r");
    if (fd != nullptr) 
    { 
      fclose(fd);
      return path; 
    }
  } 
  return "";
}
  
} /* tna */ 
