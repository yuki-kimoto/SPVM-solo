#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_NAME,
  SPerl_TYPE_C_CODE_ARRAY,
};

extern const char* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  SPerl_RESOLVED_TYPE* resolved_type;
  union {
    SPerl_TYPE_COMPONENT_NAME* type_component_name;
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array;
  } uv;
  int32_t code;
};


SPerl_TYPE* SPerl_TYPE_new(SPerl* sperl);
void SPerl_TYPE_build_parts(SPerl* sperl, SPerl_TYPE* type, SPerl_ARRAY* parts);
_Bool SPerl_TYPE_resolve_type(SPerl* sperl, SPerl_OP* op_type, uint32_t name_length);

#endif
