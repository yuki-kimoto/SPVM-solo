#ifndef SPERL_FIELD_H
#define SPERL_FIELD_H

#include "sperl_base.h"

// Field information
struct SPerl_field {
  SPerl_OP* op_type;
  SPerl_OP* op_name;
  const char* abs_name;
  size_t constant_pool_address;
  size_t abs_name_constant_pool_address;
  size_t package_byte_offset;
};

SPerl_FIELD* SPerl_FIELD_new(SPerl* sperl);
size_t SPerl_FIELD_get_byte_size(SPerl* sperl, SPerl_FIELD* field);

#endif
