#ifndef SPERL_RESOLVED_TYPE_H
#define SPERL_RESOLVED_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_RESOLVED_TYPE_C_CODE_CORE,
  SPerl_RESOLVED_TYPE_C_CODE_CLASS,
  SPerl_RESOLVED_TYPE_C_CODE_ARRAY,
  SPerl_RESOLVED_TYPE_C_CODE_SUB,
};

extern SPerl_char* const SPerl_RESOLVED_TYPE_C_CODE_NAMES[];

struct SPerl_resolved_type {
  SPerl_int code;
  SPerl_ARRAY* part_names;
  SPerl_char* name;
  SPerl_int name_length;
  SPerl_int id;
  SPerl_PACKAGE* package;
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl_PARSER* parser);
SPerl_boolean SPerl_RESOLVED_TYPE_is_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
SPerl_boolean SPerl_RESOLVED_TYPE_contain_sub(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
SPerl_boolean SPerl_RESOLVED_TYPE_is_integral(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
SPerl_boolean SPerl_RESOLVED_TYPE_is_core_type_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
SPerl_boolean SPerl_RESOLVED_TYPE_is_multi_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);
SPerl_boolean SPerl_RESOLVED_TYPE_is_core_type(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type);

#endif
