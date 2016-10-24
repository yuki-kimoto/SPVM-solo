#ifndef SPERL_ENUM_VALUE_INFO_H
#define SPERL_ENUM_VALUE_INFO_H

#include "sperl_core_type.h"

// Field information
struct SPerl_enum_value_info {
  SPerl_WORD_INFO* name_word_info;
  SPerl_CONST_VALUE* value;
  SPerl_OP* op;
};

SPerl_ENUM_VALUE_INFO* SPerl_ENUM_VALUE_INFO_new(SPerl_PARSER* parser);

#endif
