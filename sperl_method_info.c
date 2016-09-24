#include <string.h>
#include <stdio.h>

#include "sperl_type.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new() {
  SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)malloc(sizeof(SPerl_METHOD_INFO));
  memset(method_info, 0, sizeof(SPerl_METHOD_INFO));
  
  method_info->argument_infos = SPerl_ARRAY_new(0);
  
  return method_info;
}

