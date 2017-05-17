#ifndef SPVM_DATA_OBJECT_HEADER_H
#define SPVM_DATA_OBJECT_HEADER_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_object_header {
  SPVM_VALUE array_length_or_sv;
  int64_t ref_count;
  int32_t byte_size;
  int8_t type;
};

enum {
  SPVM_DATA_OBJECT_HEADER_C_TYPE_OBJECT,
  SPVM_DATA_OBJECT_HEADER_C_TYPE_STRING,
  SPVM_DATA_OBJECT_HEADER_C_TYPE_ARRAY_NUMERIC,
  SPVM_DATA_OBJECT_HEADER_C_TYPE_ARRAY_STRING,
};

#endif
