#include <stdlib.h>

#include "sperl_const_info.h"

SPerl_CONST_INFO* SPerl_CONST_INFO_new() {
  return (SPerl_CONST_INFO*)calloc(1, sizeof(SPerl_CONST_INFO));
}
