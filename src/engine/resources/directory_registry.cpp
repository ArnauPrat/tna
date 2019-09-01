
#include "directory_registry.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

namespace tna 
{

#define _TNA_MAX_NUM_DIRECTORIES 1024

uint32_t           m_buffer_capacity = 2048;
char*              m_buffer = nullptr;
uint32_t           m_num_directories = 0;
static const char* p_directories[_TNA_MAX_NUM_DIRECTORIES];


void
directory_registry_init()
{
  m_buffer = new char[m_buffer_capacity];
}

void
directory_registry_release()
{

  if(m_buffer)
  {
    delete [] m_buffer;
    m_buffer = nullptr;
  }

}

void 
directory_registry_insert(const char* directory) 
{
  assert(m_num_directories < _TNA_MAX_NUM_DIRECTORIES && "Maximum number of resource directories exceeded");
  char* tmp = new char[strlen(directory)+1];
  strcpy(tmp, directory);
  p_directories[m_num_directories] = tmp;
  m_num_directories++;
}

uint32_t 
directory_registry_get_path(const char* resource_name,
                            char* buffer,
                            uint32_t buffer_length) 
{
  for(uint32_t i = 0; i < m_num_directories; ++i) 
  {
    uint32_t written = 0;
    while ((written = snprintf(m_buffer,
                               m_buffer_capacity,
                               "%s/%s",
                               p_directories[i],
                               resource_name)) >= m_buffer_capacity)
    {
      uint32_t new_capacity = m_buffer_capacity*2;
      char* tmp = new char[new_capacity];
      memcpy(tmp, m_buffer, sizeof(char)*m_buffer_capacity);
      delete [] m_buffer;
      m_buffer = tmp;
      m_buffer_capacity = new_capacity;
    }

    FILE* fd = fopen(m_buffer,"r");
    if (fd != nullptr) 
    { 
      fclose(fd);
      strncpy(buffer, m_buffer, buffer_length);
      return strlen(m_buffer);
    }
  } 
  return 0;
}
  
} /* tna */ 
