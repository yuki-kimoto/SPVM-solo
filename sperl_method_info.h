#ifndef SPERL_METHOD_H
#define SPERL_METHOD_H

#include "sperl_type.h"

// Method information
struct SPerl_method_info {
  SPerl_WORD_INFO* name;
  SPerl_WORD_INFO* return_type;
  SPerl_ARRAY* descripters;
  SPerl_int argument_count;
  SPerl_ARRAY* my_var_infos;
  SPerl_OP* op_block;
  SPerl_OP* op;
  SPerl_CLASS_INFO* class_info;
};

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new(SPerl_PARSER* parser);

#endif
