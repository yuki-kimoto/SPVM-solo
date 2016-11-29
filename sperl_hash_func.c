#include <string.h>
#include <stdlib.h>

#include "sperl_hash_func.h"

SPerl_long SPerl_HASH_FUNC_calc_hash(SPerl_uchar* str, SPerl_int len) {
  SPerl_uchar* str_tmp = str;
  SPerl_long hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + *str_tmp++;
  }
  
  return (uint32_t)hash;
}
