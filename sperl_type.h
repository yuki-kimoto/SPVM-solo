#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_NAME,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB,
};

extern uint8_t* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  int32_t code;
  SPerl_RESOLVED_TYPE* resolved_type;
  union {
    SPerl_TYPE_COMPONENT_NAME* type_component_name;
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array;
    SPerl_TYPE_COMPONENT_SUB* type_component_sub;
  } uv;
};


SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);
void SPerl_TYPE_build_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts);
_Bool SPerl_TYPE_resolve_type(SPerl_PARSER* parser, SPerl_OP* op_type, int32_t name_length);

#endif
