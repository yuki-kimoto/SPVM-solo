#ifndef SPERL_SUB_H
#define SPERL_SUB_H

#include "sperl_base.h"

// Method information
struct SPerl_sub {
  SPerl_OP* op_package;
  SPerl_OP* op_name;
  SPerl_OP* op_return_type;
  SPerl_ARRAY* op_descripters;
  SPerl_ARRAY* op_my_vars;
  SPerl_int argument_count;
  SPerl_OP* op_block;
  SPerl_boolean anon;
  SPerl_int id;
  SPerl_ARRAY* op_constants;
  SPerl_CONSTANT_POOL* constant_pool;
};

SPerl_SUB* SPerl_SUB_new(SPerl_PARSER* parser);

#endif
