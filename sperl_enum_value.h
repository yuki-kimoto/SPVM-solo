#ifndef SPERL_ENUM_VALUE_H
#define SPERL_ENUM_VALUE_H

#include "sperl_base.h"

// Field information
struct SPerl_enum_value {
  SPerl_WORD* name_word;
  SPerl_OP* op_constant;
};

SPerl_ENUM_VALUE* SPerl_ENUM_VALUE_new(SPerl_PARSER* parser);

#endif
