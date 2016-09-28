#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"

// Class information
struct SPerl_class_info {
  SPerl_char* name;
  SPerl_char desc_flags;
  SPerl_ARRAY* field_infos;
  SPerl_ARRAY* method_infos;
  SPerl_ARRAY* const_infos;
  SPerl_OP* op_block;
};

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new();

#endif
