#include <string.h>
#include <stdlib.h>

#include "sperl_hash_func.h"

uint32_t SPerl_HASH_FUNC_calc_hash(const char* str, int32_t len) {
  const char* str_tmp = str;
  uint32_t hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + (uint8_t) *str_tmp++;
  }
  
  return hash;
}
