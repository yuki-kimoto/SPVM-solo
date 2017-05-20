#ifndef SPVM_DATA_HEADER_ARRAY_STRING_H
#define SPVM_DATA_HEADER_ARRAY_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_header_array_string {
  int8_t type;
  int16_t byte_size;
  int32_t ref_count;
  int32_t length;
};

#endif
