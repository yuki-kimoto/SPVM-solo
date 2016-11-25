#ifndef SPERL_NAME_H
#define SPERL_NAME_H

#include "sperl_base.h"

enum {
  SPerl_NAME_C_CODE_SUB,
  SPerl_NAME_C_CODE_FIELD,
  SPerl_NAME_C_CODE_ENUM
};

extern SPerl_char* const SPerl_NAME_C_CODE_NAMES[];

// Field information
struct SPerl_name {
  SPerl_int code;
  SPerl_OP* op_var;
  SPerl_OP* op_base_name;
  SPerl_OP* op_abs_name;
  SPerl_int argument_count;
  SPerl_boolean anon;
  SPerl_char* complete_name;
};

SPerl_NAME* SPerl_NAME_new(SPerl_PARSER* parser);

#endif
