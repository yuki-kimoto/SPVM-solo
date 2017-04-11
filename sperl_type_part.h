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
  union {
    SPerl_OP* op_name;
    const char* char_name;
  } uv;
  int32_t code;
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl* sperl);

#endif
