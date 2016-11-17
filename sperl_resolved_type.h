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
  SPerl_int id;
  union {
    SPerl_BODY_CLASS* body_class;
    SPerl_BODY_CORE* body_core;
  } uv;
};

#endif
