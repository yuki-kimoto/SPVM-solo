#ifndef SPERL_FIELD_H
#define SPERL_FIELD_H

#include "sperl_base.h"

// Field information
struct SPerl_field {
  SPerl_OP* op_package;
  SPerl_OP* op_name;
  SPerl_OP* op_type;
  int32_t id;
  int32_t constant_pool_address;
  int32_t package_byte_offset;
};

SPerl_FIELD* SPerl_FIELD_new(SPerl* sperl);
int32_t SPerl_FIELD_get_byte_size(SPerl* sperl, SPerl_FIELD* field);

#endif
