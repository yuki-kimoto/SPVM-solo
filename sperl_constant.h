#ifndef SPerl_CONSTANT_H
#define SPerl_CONSTANT_H

#include "sperl_base.h"

enum {
  SPerl_CONSTANT_C_CODE_BOOLEAN,
  SPerl_CONSTANT_C_CODE_BYTE,
  SPerl_CONSTANT_C_CODE_INT,
  SPerl_CONSTANT_C_CODE_LONG,
  SPerl_CONSTANT_C_CODE_FLOAT,
  SPerl_CONSTANT_C_CODE_DOUBLE,
  SPerl_CONSTANT_C_CODE_STRING,
};

extern SPerl_char* const SPerl_CONSTANT_C_CODE_NAMES[];

struct SPerl_constant {
  SPerl_int code;
  union {
    SPerl_int int_value;
    SPerl_int long_value;
    float float_value;
    SPerl_double double_value;
    SPerl_char* string_value;
  } uv;
  SPerl_RESOLVED_TYPE* resolved_type;
  SPerl_int pool_pos;
};

SPerl_CONSTANT* SPerl_CONSTANT_new(SPerl_PARSER* parser);
SPerl_CONSTANT* SPerl_CONSTANT_create_int_1(SPerl_PARSER* parser);

#endif
