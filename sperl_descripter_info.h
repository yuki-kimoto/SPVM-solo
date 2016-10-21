#ifndef SPERL_DESCRIPTER_INFO_H
#define SPERL_DESCRIPTER_INFO_H

#include "sperl_core_type.h"

enum SPerl_descripter_info_type {
  SPerl_DESCRIPTER_INFO_TYPE_CONST,
  SPerl_DESCRIPTER_INFO_TYPE_STATIC,
  SPerl_DESCRIPTER_INFO_TYPE_VALUE,
  SPerl_DESCRIPTER_INFO_TYPE_ENUM
};

extern SPerl_char* const SPerl_DESCRIPTER_INFO_type_names[];

// Field information
struct SPerl_descripter_info {
  SPerl_char type;
  SPerl_OP* op;
};

SPerl_DESCRIPTER_INFO* SPerl_DESCRIPTER_INFO_new(SPerl_PARSER* parser);

#endif
