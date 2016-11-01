#ifndef SPERL_TYPE_PART_H
#define SPERL_TYPE_PART_H

#include "sperl_base.h"

enum {
  SPerl_TYPE_PART_C_CODE_SUB,
  SPerl_TYPE_PART_C_CODE_WORD,
  SPerl_TYPE_PART_C_CODE_CHAR
};

extern SPerl_char* const SPerl_TYPE_PART_C_CODE_NAMES[];

struct SPerl_type_part {
  SPerl_char code;
  union {
    SPerl_WORD* name_word;
    SPerl_char c;
  } uv;
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl_PARSER* parser);

#endif
