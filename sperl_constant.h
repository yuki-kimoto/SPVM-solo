#ifndef SPerl_CONSTANT_H
#define SPerl_CONSTANT_H

#include "sperl_base.h"

enum {
  SPerl_CONSTANT_C_CODE_BOOLEAN,
  SPerl_CONSTANT_C_CODE_INT,
  SPerl_CONSTANT_C_CODE_LONG,
  SPerl_CONSTANT_C_CODE_FLOAT,
  SPerl_CONSTANT_C_CODE_DOUBLE,
  SPerl_CONSTANT_C_CODE_STRING,
  SPerl_CONSTANT_C_CODE_UTF8,
};

extern const char* const SPerl_CONSTANT_C_CODE_NAMES[];

struct SPerl_constant {
  SPerl_RESOLVED_TYPE* resolved_type;
  union {
    int8_t byte_value;
    int16_t short_value;
    int32_t int_value;
    int64_t long_value;
    float float_value;
    double double_value;
    const char* string_value;
  } uv;
  int32_t code;
  int32_t address;
  char* num_str;
  _Bool is_undef;
};

SPerl_CONSTANT* SPerl_CONSTANT_new(SPerl* sperl);
SPerl_CONSTANT* SPerl_CONSTANT_create_int_1(SPerl* sperl);

#endif
