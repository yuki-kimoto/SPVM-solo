#ifndef SPERL_METHOD_H
#define SPERL_METHOD_H

#include "sperl_type.h"

// Method information
struct SPerl_method {
  SPerl_char* name;
  SPerl_char desc_flags;
  SPerl_char* return_type;
  SPerl_ARGUMENT* argument;
  SPerl_long argument_count;
  SPerl_OP* op_block;
};

SPerl_METHOD* SPerl_METHOD_new();

#endif
