/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/

#include "proc_read.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>




static void *
__get_ptr_from_str(const char *str)
{
  unsigned long long addr;

  errno = 0;
  addr  = strtoull(str, 0, 16);
  if(errno) {
    perror("strtoull: ");
    exit(EXIT_FAILURE);
  }
  return (void *)addr;
}



int
open_pmaps_file(struct program_info *pinfo, pid_t pid)
{
  char buff[200];
  sprintf(buff, "/proc/%d/maps", pid);
  int val = open(buff, O_RDONLY);
  pinfo->pid = val;
  if (errno){
    return -1;
  }
  return 0;
}

int
parse_pmaps_file(struct program_info *pinfo)
{
  int fd = pinfo->pid;
  ssize_t bytesRead;
  while((bytesRead = read(fd, pinfo->infobuff, sizeof(pinfo->infobuff) - 1))){
	  if (bytesRead == -1){
                if (errno == EINTR){
                        //this is not terrible, continue
                        continue;
                }
                else{ //actual failure
                        return -1;
                }
        }
  }
  return 0;
}

void *
get_code_start(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == 'x' && pinfo->infobuff[i-1] == '-' && pinfo->infobuff[i-2] == 'r'){
      strncpy(buff, (pinfo->infobuff + i - 28), 12);
      buff[12] = '\0';
      break;
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_code_end(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == 'x' && pinfo->infobuff[i-1] == '-' && pinfo->infobuff[i-2] == 'r'){
      strncpy(buff, (pinfo->infobuff + i - 15), 12);
      buff[12] = '\0';
      break;
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_globals_start(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == 'w' && pinfo->infobuff[i+1] == '-' && pinfo->infobuff[i-1] == 'r'){
      strncpy(buff, (pinfo->infobuff + i - 27), 12);
      buff[12] = '\0';
      break;
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_globals_end(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == 'w' && pinfo->infobuff[i+1] == '-' && pinfo->infobuff[i-1] == 'r'){
      strncpy(buff, (pinfo->infobuff + i - 14), 12);
      buff[12] = '\0';
      break;
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_stack_start(struct program_info *pinfo)
{
  char buff[13] = {0}; 
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == '['){
      if (pinfo->infobuff[i+1] == 's'){
        strncpy(buff, (pinfo->infobuff + i - 74), 12);
	buff[12] = '\0';
      }
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_stack_end(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == '['){
      if (pinfo->infobuff[i+1] == 's'){
        strncpy(buff, (pinfo->infobuff + i - 60), 12);
        buff[12] = '\0';
      }
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_heap_start(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == '['){
      if (pinfo->infobuff[i+1] == 'h'){
        strncpy(buff, (pinfo->infobuff + i - 74), 12);
        buff[12] = '\0';
      }
    }
  }
  return __get_ptr_from_str(buff);
}

void *
get_heap_end(struct program_info *pinfo)
{
  char buff[13] = {0};
  for (size_t i = 0; i < strlen(pinfo->infobuff); i++){
    if (pinfo->infobuff[i] == '['){
      if (pinfo->infobuff[i+1] == 'h'){
        strncpy(buff, (pinfo->infobuff + i - 60), 12);
        buff[12] = '\0';
      }
    }
  }
  return __get_ptr_from_str(buff);
}
