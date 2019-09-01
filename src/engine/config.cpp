
#include "common.h"
#include "config.h"
#include "resources/directory_registry.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>



namespace tna 
{

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
config_init(config_t* config, const char* file_name) 
{
  FILE* fd = fopen(file_name, "r");
  if(fd == NULL)
  {
    TNA_LOG_ERROR("Configuration file %s does not exist", file_name);
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
      TNA_LOG_ERROR("Ill-formed configuration file entry at line %d", line_count);
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
      TNA_LOG_ERROR("Ill-formed configuration file entry at line %d", line_count);
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
      config_insert_vk_layer(config, value);
    }

    if (strcmp(option,"ResourceDirectory") == 0)
    {
      directory_registry_insert(value); 
    }

    TNA_LOG_ERROR("Parsed option %s with value %s", option, value);
    line_count++;
  }
  fclose(fd);
  free(buffer);
  free(option);
  free(value);
  return;
} 

void
config_release(config_t* config)
{
    for(uint32_t i = 0; i < config->m_num_vk_vlayers; ++i)
    {
      delete [] config->m_vk_vlayers[i];
    }
    config->m_num_vk_vlayers = 0;
}


void
config_insert_vk_layer(config_t* config, 
                       const char* layer_name)
{
  assert(config->m_num_vk_vlayers < _TNA_MAX_VK_VALIDATION_LAYERS && "Maximum number of validarion layers exceeded");

  uint32_t layer_name_length = strlen(layer_name);
  config->m_vk_vlayers[config->m_num_vk_vlayers] = new char[layer_name_length+1];
  strcpy(config->m_vk_vlayers[config->m_num_vk_vlayers], layer_name);
  config->m_num_vk_vlayers++;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////



}
