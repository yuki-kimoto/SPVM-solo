#ifndef SPERL_ARGUMENT_INFO_H
#define SPERL_ARGUMENT_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_argument_info {
  SPerl_char* type;
  SPerl_char desc_flag;
  SPerl_char* name;
};

#endif
