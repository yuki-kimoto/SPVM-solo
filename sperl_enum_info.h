#ifndef SPERL_ENUM_INFO_H
#define SPERL_ENUM_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_enum_info {
  SPerl_WORD_INFO* type;
  SPerl_WORD_INFO* name;
  SPerl_ARRAY* descripters;
  SPerl_OP* op;
  SPerl_CLASS_INFO* class_info;
};

SPerl_ENUM_INFO* SPerl_ENUM_INFO_new(SPerl_PARSER* parser);

#endif
