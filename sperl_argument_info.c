#include <stdlib.h>
#include <string.h>

#include "sperl_argument_info.h"

SPerl_ARGUMENT_INFO* SPerl_ARGUMENT_INFO_new() {
  SPerl_ARGUMENT_INFO* argument_info = (SPerl_ARGUMENT_INFO*)malloc(sizeof(SPerl_ARGUMENT_INFO));
  memset(argument_info, 0, sizeof(SPerl_ARGUMENT_INFO));
  return argument_info;
}
