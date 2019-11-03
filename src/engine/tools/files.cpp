

#include "../common.h"
#include "files.h"

namespace tna 
{

std::vector<char> 
read_file(const std::string& filename) 
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) 
  {
    TNA_LOG_ERROR(TNA_ERROR::E_IO_FILE_NOT_FOUND, 
                  "failed to open file %s",filename.c_str());
  }

  size_t fileSize = (size_t) file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

}
