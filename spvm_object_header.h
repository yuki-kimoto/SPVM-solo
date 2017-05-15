#ifndef SPVM_OBJECT_HEADER_H
#define SPVM_OBJECT_HEADER_H

#include "spvm_base.h"

struct SPVM_object_header {
  union {
    int8_t* byte_values;
    int16_t* short_values;
    int32_t* int_values;
    int64_t* long_values;
    float* float_values;
    double* double_values;
    SPVM_SV** sv_values;
  } uv;
  intmax_t array_length_or_sv;
  int64_t ref_count;
  int32_t byte_size;
  int8_t type;
  int8_t array_type;
};

enum {
  SPVM_OBJECT_HEADER_C_TYPE_OBJECT,
  SPVM_OBJECT_HEADER_C_TYPE_ARRAY,
  SPVM_OBJECT_HEADER_C_TYPE_STRING
};

enum {
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_BYTE,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_SHORT,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_INT,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_LONG,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_FLOAT,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_DOUBLE,
  SPVM_OBJECT_HEADER_C_ARRAY_TYPE_STRING,
};

#endif
