#ifndef SPVM_FIELD_H
#define SPVM_FIELD_H

#include "spvm_base.h"

// Field information
struct SPVM_field {
  SPVM_OP* op_type;
  SPVM_OP* op_name;
  const char* abs_name;
  int32_t constant_pool_index;
  int32_t abs_name_constant_pool_index;
  int32_t name_constant_pool_index;
  int32_t index;
};

SPVM_FIELD* SPVM_FIELD_new(SPVM_COMPILER* compiler);
int32_t SPVM_FIELD_get_byte_size(SPVM_COMPILER* compiler, SPVM_FIELD* field);

#endif
