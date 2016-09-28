#ifndef SPERL_VARIABLE_INFO_H
#define SPERL_VARIABLE_INFO_H

#include "sperl_type.h"

struct SPerl_variable_info {
  SPerl_char* type;
  SPerl_char desc_flags;
  SPerl_char* name;
}

#endif
