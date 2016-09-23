#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include "sperl_type.h"

#define SPerl_CLASS_MAX 1024

#define SPerl_DESC_CONST 1
#define SPerl_DESC_STATIC 2

// Method information
struct SPerl_method_info {
  SPerl_char* name;
  SPerl_char desc_flag;
  SPerl_char* return_type;
  SPerl_ARGUMENT_INFO* argument_infos;
  SPerl_long argument_count;
  SPerl_OP* op_block;
};

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new();

// Class information
struct SPerl_class_info {
  SPerl_char* name;
  SPerl_short desc_flag;
  SPerl_char* super_class_name;
  SPerl_FIELD_INFO* field_infos;
  SPerl_long field_count;
  SPerl_METHOD_INFO* method_infos;
  SPerl_long method_count;
  SPerl_long* local_var;
};

extern SPerl_CLASS_INFO* class_infos;

SPerl_char SPerl_get_desc_flag(SPerl_char* desc_str);

#endif
