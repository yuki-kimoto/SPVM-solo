#include <stdlib.h>

#include "sperl_type.h"
#include "sperl_var_info.h"

SPerl_VAR_INFO* SPerl_VAR_INFO_new() {
  return (SPerl_VAR_INFO*)calloc(1, sizeof(SPerl_VAR_INFO));
}
