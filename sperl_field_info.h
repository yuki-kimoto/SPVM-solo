#ifndef SPERL_FIELD_INFO_H
#define SPERL_FIELD_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_field_info {
  SPerl_char* type;
  SPerl_char desc_flags;
  SPerl_char* name;
  SPerl_CLASS_INFO* class_info;
  SPerl_OP* op;
};

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new(SPerl_PARSER* parser);

#endif
