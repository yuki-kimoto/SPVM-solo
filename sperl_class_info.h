#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"

// Class information
struct SPerl_class_info {
  SPerl_char* name;
  SPerl_char desc_flags;
  SPerl_FIELD_INFO* field_infos;
  SPerl_int field_count;
  SPerl_int method_base_pos;
  SPerl_int method_count;
};

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new();

#endif
