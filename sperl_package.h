#ifndef SPERL_PACKAGE_H
#define SPERL_PACKAGE_H

#include "sperl_base.h"

struct SPerl_package {
  SPerl_OP* op_name;
  SPerl_OP* op_type;
  int32_t size;
  SPerl_ARRAY* op_fields;
  SPerl_ARRAY* op_subs;
  SPerl_ARRAY* op_constants;
  SPerl_CONSTANT_POOL* constant_pool;
};

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl_PARSER* parser);

#endif
