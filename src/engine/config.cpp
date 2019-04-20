
#include "common.h"
#include "config.h"
#include "resources/directory_registry.h"
#include <stdio.h>
#include <string.h>


#define TNA_VK_VALIDATION_LAYERS_GROWTH 8

namespace tna 
{

Config::Config() : 
m_viewport_width(1440),
m_viewport_height(900),
m_fullscreen(false),
m_max_vk_vlayers(0),
m_num_vk_vlayers(0),
m_vk_vlayers(nullptr)
{

}

Config::~Config()
{
  if(m_vk_vlayers != NULL)
  {
    for(uint32_t i = 0; i < m_num_vk_vlayers; ++i)
    {
      delete m_vk_vlayers[i];
    }
    free(m_vk_vlayers);
    m_max_vk_vlayers = 0;
    m_num_vk_vlayers = 0;
    m_vk_vlayers = NULL;
  }
}


void
Config::insert_vk_layer(const std::string& layer_name)
{

  if(m_num_vk_vlayers == m_max_vk_vlayers)
  {
    m_max_vk_vlayers += TNA_VK_VALIDATION_LAYERS_GROWTH;
    m_vk_vlayers = (std::string**)realloc(m_vk_vlayers, sizeof(std::string*)*m_max_vk_vlayers);
  }
  m_vk_vlayers[m_num_vk_vlayers] = new std::string(layer_name);
  m_num_vk_vlayers++;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


static int 
fread_line(FILE* fd, char** buffer, size_t* buffer_size)
{

  size_t curr = 0;
  int c = getc(fd) ;
  while(c != '\n' && c != EOF)
  {
    if(curr == (*buffer_size)-1)
    {
      *buffer_size *=2;
      *buffer = (char*)realloc(*buffer,*buffer_size);
    }
    (*buffer)[curr] = c;
    curr++;
    c = getc(fd);
  }
  (*buffer)[curr] = '\0';
  return curr;
}

void 
load_config(const std::string& configFileName, Config* config) 
{
  FILE* fd = fopen(configFileName.c_str(), "r");
  if(fd == NULL)
  {
    log->error("Configuration file %s does not exist", configFileName.c_str());
    report_error(TNA_ERROR::E_SUCCESS);
  }

  size_t buffer_size = 512;
  size_t option_size = 256;
  size_t value_size = 256;
  char* buffer = (char*)malloc(sizeof(char)*buffer_size);
  char* option = (char*)malloc(sizeof(char)*option_size);
  char* value = (char*)malloc(sizeof(char)*value_size);
  uint32_t line_count = 0;
  while (fread_line(fd, &buffer, &buffer_size) > 0) 
  {
    if(buffer[0] == '#')
    {
      continue;
    }
    char* tok = strtok(buffer," \t" );
    if(tok == NULL)
    {
      log->error("Ill-formed configuration file entry at line %d", line_count);
      report_error(TNA_ERROR::E_IO_UNEXPECTED_INPUT_ERROR);
    }

    size_t len = strlen(tok);
    if (len > option_size-1)
    {
      option_size *= 2;
      option = (char*)realloc(option, option_size);
    }
    memcpy(option, tok, sizeof(char)*(len+1));
    printf("option: %s\n",option);

    tok = strtok(NULL," \t");
    if(tok == NULL)
    {
      log->error("Ill-formed configuration file entry at line %d", line_count);
      report_error(TNA_ERROR::E_IO_UNEXPECTED_INPUT_ERROR);
    }

    len = strlen(tok);
    if (len > value_size-1)
    {
      value_size *= 2;
      value = (char*)realloc(value, value_size);
    }
    memcpy(value, tok, sizeof(char)*(len+1));

    printf("value: %s\n",value);


    if (strcmp(option,"ViewportWidth") == 0) 
    {
      config->m_viewport_width = atoi(value);
    }

    if (strcmp(option,"ViewportHeight") == 0) 
    {
      config->m_viewport_height = atoi(value);
    }

    if (strcmp(option,"FullScreen") == 0)
    {
      config->m_fullscreen = atoi(value);
    }

    if (strcmp(option,"VkValidationLayer") == 0) 
    {
      config->insert_vk_layer(std::string(value));
    }

    if (strcmp(option,"ResourceDirectory") == 0)
    {
      register_directory(value); 
    }

    log->log("Parsed option %s with value %s", option, value);
    line_count++;
  }
  fclose(fd);
  free(buffer);
  free(option);
  free(value);
  return;
} 
}
