/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Implementation of the memory area with several types.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

// The length of the header we are using.
#define HLEN 2 * sizeof(int)

/**
 * Implementation of getmem()
 */
void *
getmem(int nc, int ni){
  void* mem = malloc(8 + nc + ni*sizeof(int));
  *(int*)mem = ni;
  *(int*)(mem + 4) = nc;
  return mem + 8;
}

/**
 * Implementation of freemem()
 */
void
freemem(void *mem)
{
  free(mem-8); //this is for sure right
}

/**
 * Implementation of getnc()
 */
int
getnc(void *mem)
{
  return *(int*)(mem-4);
}

/**
 * Implementation of getni()
 */
int
getni(void *mem)
{
  return *(int*)(mem-8);
}

/**
 * Implementation of getstr()
 */
char *
getstr(void *mem)
{
  int ni = getni(mem);
  return (char*)mem + ni * 4;
}

/**
 * Implementation of getintptr()
 */
int *
getintptr(void *mem)
{
  return (int*)mem;
}
/**
 * Implementation of getint_at()
 */
int
getint_at(void *mem, int idx, int *res)
{
  int ni = getni(mem);
  
  if (idx < 0 || idx >= ni){
	return -1;
  }
  
  *res = *((int*)mem+idx);
  return 0;
}

/**
 * Implementation of setint_at()
 */
int
setint_at(void *mem, int idx, int val)
{
  int ni = getni(mem);

  if (idx < 0 || idx >= ni){
        return -1;
  }

  *((int*)mem+idx) = val;
  return 0;
}

/**
 * Implementation of cpstr()
 */
size_t
cpstr(void *mem, const char *str, size_t len)
{
  int nc = getnc(mem);
  if (len < nc){
	strcpy((char*)mem, str);
	return len+1;
  }
  else{
	
	for (int i = 0; i < nc; i++){
		*((char*)mem+i) = str[i];
	}
	*((char*)mem+nc) = '\0';
	return nc;
  }
}
