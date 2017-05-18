#ifndef SPVM_DATA_HEADER_STRING_H
#define SPVM_DATA_HEADER_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_header_string {
  SPVM_VALUE array_length_or_sv;
  int64_t ref_count;
  int32_t byte_size;
  int8_t type;
};

#endif
