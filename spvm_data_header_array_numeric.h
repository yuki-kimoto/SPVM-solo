#ifndef SPVM_DATA_HEADER_ARRAY_NUMERIC_H
#define SPVM_DATA_HEADER_ARRAY_NUMERIC_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_header_array_numeric {
  int8_t type;
  int32_t ref_count;
  int64_t length;
  int32_t byte_size;
};

#endif
