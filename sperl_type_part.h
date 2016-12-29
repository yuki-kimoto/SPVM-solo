#ifndef SPERL_TYPE_PART_H
#define SPERL_TYPE_PART_H

#include "sperl_base.h"

enum {
  SPerl_TYPE_PART_C_CODE_SUB,
  SPerl_TYPE_PART_C_CODE_NAME,
  SPerl_TYPE_PART_C_CODE_BYTE
};

extern const char* const SPerl_TYPE_PART_C_CODE_NAMES[];

struct SPerl_type_part {
  int32_t code;
  union {
    SPerl_OP* op_name;
    const char* char_name;
  } uv;
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl_PARSER* parser);

#endif
