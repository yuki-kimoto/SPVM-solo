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
};

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new();
void SPerl_METHOD_INFO_dump_method_infos(SPerl_ARRAY* method_infos);

#endif
