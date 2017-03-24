#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_hash_func.h"

int64_t SPerl_HASH_FUNC_calc_hash(SPerl* sperl, const char* str, int64_t len) {
  (void)sperl;
  
  assert(len > 0);
  
  const char* str_tmp = str;
  uint32_t hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + (uint8_t) *str_tmp++;
  }
  
  return (int64_t)hash;
}
