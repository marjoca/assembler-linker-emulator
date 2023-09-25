#include "../inc/pomocni.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* copy_string(const char *str){
  size_t len=strnlen(str,32);
  char* buffer=(char*)malloc(len+1);
  strncpy(buffer,str,len);
  buffer[len]='\0';
  return buffer;

}