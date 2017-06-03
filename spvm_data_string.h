#ifndef SPVM_DATA_STRING_H
#define SPVM_DATA_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_string {
  int32_t data_count;
  int8_t type;
  SPVM_SV* sv;
};

#endif

