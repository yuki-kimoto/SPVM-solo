#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include "sperl_type.h"

#define SPerl_CLASS_MAX 1024


// Class information
struct SPerl_class_info {
  SPerl_char* name;
  SPerl_short desc_flag;
  SPerl_char* super_class_name;
  SPerl_FIELD_INFO* field_infos;
  SPerl_int field_count;
  SPerl_METHOD_INFO* method_infos;
  SPerl_int method_count;
  SPerl_int* local_var;
};

extern SPerl_CLASS_INFO* class_infos;


#endif
