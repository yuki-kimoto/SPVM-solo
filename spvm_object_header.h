#ifndef SPVM_OBJECT_HEADER_H
#define SPVM_OBJECT_HEADER_H

#include "spvm_base.h"

struct SPVM_object_header {
  int8_t type;
  int8_t array_type;
  int8_t freelist_index;
  int8_t padding3;
  int32_t byte_size;
  int64_t ref_count;
  intmax_t array_length_or_sv;
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
