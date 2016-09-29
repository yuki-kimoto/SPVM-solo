#ifndef SPERL_METHOD_H
#define SPERL_METHOD_H

#include "sperl_type.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"

// Method information
struct SPerl_method_info {
  SPerl_char* name;
  SPerl_char desc_flags;
  SPerl_char* return_type;
  SPerl_ARRAY* argument_infos;
  SPerl_int argument_count;
  SPerl_OP* op_block;
  SPerl_ARRAY* my_var_infos;
};

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new();

#endif
