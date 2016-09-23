#ifndef SPERL_FIELD_INFO_H
#define SPERL_FIELD_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_field_info {
  SPerl_char* type;
  SPerl_short desc_flag;
  SPerl_char* name;
};

#endif
