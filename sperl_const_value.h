#ifndef SPerl_CONST_VALUE_H
#define SPerl_CONST_VALUE_H

#include "sperl_base.h"

enum {
  SPerl_CONST_VALUE_C_CODE_BOOLEAN,
  SPerl_CONST_VALUE_C_CODE_CHAR,
  SPerl_CONST_VALUE_C_CODE_INT,
  SPerl_CONST_VALUE_C_CODE_LONG,
  SPerl_CONST_VALUE_C_CODE_FLOAT,
  SPerl_CONST_VALUE_C_CODE_DOUBLE,
  SPerl_CONST_VALUE_C_CODE_STRING
};

struct SPerl_const_value {
  SPerl_int code;
  union {
    SPerl_int int_value;
    SPerl_int long_value;
    float float_value;
    SPerl_double double_value;
    SPerl_char* string_value;
  } uv;
  SPerl_TYPE* type;
  SPerl_int pool_pos;
  SPerl_OP* op;
};

SPerl_CONST_VALUE* SPerl_CONST_VALUE_new(SPerl_PARSER* parser);
SPerl_CONST_VALUE* SPerl_CONST_VALUE_copy(SPerl_PARSER* parser, SPerl_CONST_VALUE* const_value);

#endif
