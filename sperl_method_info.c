#include <string.h>
#include <stdio.h>

#include "sperl_type.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_hash.h"

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new() {
  SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)calloc(1, sizeof(SPerl_METHOD_INFO));
  
  method_info->my_var_infos = SPerl_ARRAY_new(0);
  method_info->my_var_info_h = SPerl_HASH_new(0);
  
  return method_info;
}
