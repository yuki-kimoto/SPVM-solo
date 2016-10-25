#ifndef SPERL_METHOD_H
#define SPERL_METHOD_H

#include "sperl_core_type.h"
#include "sperl_type.h"

// Method information
struct SPerl_method {
  SPerl_WORD* name_word;
  SPerl_TYPE* return_type;
  SPerl_ARRAY* descripters;
  SPerl_int argument_count;
  SPerl_ARRAY* my_vars;
  SPerl_OP* op_block;
  SPerl_OP* op;
  SPerl_CLASS* class;
  SPerl_boolean anon;
};

SPerl_METHOD* SPerl_METHOD_new(SPerl_PARSER* parser);

#endif
