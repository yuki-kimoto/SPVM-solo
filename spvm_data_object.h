#ifndef SPVM_DATA_OBJECT_H
#define SPVM_DATA_OBJECT_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_object {
  int8_t type;
  int32_t ref_count;
  int32_t package_constant_pool_address;
  int32_t* constant_pool;
};

#endif
