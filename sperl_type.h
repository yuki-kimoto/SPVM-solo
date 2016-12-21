#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_WORD,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB,
};

extern uint8_t* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  int32_t code;
  SPerl_ARRAY* parts;
  uint8_t* name;
  SPerl_RESOLVED_TYPE* resolved_type;
  _Bool resolved;
  union {
    SPerl_TYPE_COMPONENT_WORD* type_component_word;
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array;
    SPerl_TYPE_COMPONENT_SUB* type_component_sub;
  } uv;
};

void SPerl_TYPE_resolve_type(SPerl_PARSER* parser, SPerl_TYPE* type, int32_t name_length);

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);
void SPerl_TYPE_to_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts);
void SPerl_TYPE_build_parts(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_TYPE_print(SPerl_PARSER* parser, SPerl_TYPE* type, FILE* fh);
void SPerl_TYPE_build_name(SPerl_PARSER* parser, SPerl_TYPE* type);

#endif
