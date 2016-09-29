#include <stdlib.h>

#include "sperl_type.h"
#include "sperl_my_var_info.h"

SPerl_MY_VAR_INFO* SPerl_MY_VAR_INFO_new() {
  return (SPerl_MY_VAR_INFO*)calloc(1, sizeof(SPerl_MY_VAR_INFO));
}
