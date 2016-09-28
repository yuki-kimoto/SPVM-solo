#ifndef SPERL_FIELD_INFO_H
#define SPERL_FIELD_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_field_info {
  SPerl_char* type;
  SPerl_char desc_flags;
  SPerl_char* name;
};

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new();

#endif
