#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdio.h>

#include "sperl_base.h"

enum {
  SPerl_TYPE_C_CODE_WORD,
  SPerl_TYPE_C_CODE_ARRAY,
  SPerl_TYPE_C_CODE_SUB,
};

extern SPerl_char* const SPerl_TYPE_C_CODE_NAMES[];

struct SPerl_type {
  SPerl_char code;
  SPerl_ARRAY* parts;
  SPerl_ARRAY* resolved_part_names;
  SPerl_char* resolved_name;
  SPerl_int resolved_name_length;
  SPerl_int id;
  union {
    SPerl_TYPE_WORD* type_word;
    SPerl_TYPE_ARRAY* type_array;
    SPerl_TYPE_SUB* type_sub;
  } uv;
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser);
SPerl_TYPE* SPerl_TYPE_create_word_type(SPerl_PARSER* parser, SPerl_char* type_name);
SPerl_TYPE* SPerl_TYPE_create_array_type(SPerl_PARSER* parser, SPerl_char* type_name);
void SPerl_TYPE_to_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts);
void SPerl_TYPE_build_parts(SPerl_PARSER* parser, SPerl_TYPE* type);
void SPerl_TYPE_print(SPerl_PARSER* parser, SPerl_TYPE* type, FILE* fh);
SPerl_boolean SPerl_TYPE_is_core_type(SPerl_PARSER* parser, SPerl_int type_id);

#endif
