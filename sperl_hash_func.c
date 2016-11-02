#include <string.h>
#include <stdlib.h>

#include "sperl_hash_func.h"

SPerl_int SPerl_HASH_FUNC_calc_hash(SPerl_char* str, SPerl_int len) {
  SPerl_char* str_tmp = str;
  SPerl_int hash = 5381;
  while (len--) {
    hash = ((hash << 5) + hash) + *str_tmp++;
  }
  return hash;
}
