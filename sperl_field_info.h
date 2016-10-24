#ifndef SPERL_FIELD_INFO_H
#define SPERL_FIELD_INFO_H

#include "sperl_core_type.h"
#include "sperl_type_info.h"

// Field information
struct SPerl_field_info {
  SPerl_TYPE_INFO* type_info;
  SPerl_WORD_INFO* name_word_info;
  SPerl_ARRAY* descripter_infos;
  SPerl_OP* op;
  SPerl_CLASS_INFO* class_info;
};

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new(SPerl_PARSER* parser);

#endif
