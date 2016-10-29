#ifndef SPERL_TYPE_EXPR_H
#define SPERL_TYPE_EXPR_H

#include "sperl_base.h"

enum {
  SPerl_TYEP_EXPR_C_CODE_WORD,
  SPerl_TYPE_EXPR_C_CODE_ARRAY,
  SPerl_TYPE_EXPR_C_CODE_SUB
};

extern SPerl_char* const SPerl_TYPE_EXPR_C_CODE_NAMES[];

struct SPerl_type_expr {
  SPerl_char code;
  union {
    SPerl_WORD* name_word;
    SPerl_TYPE_EXPR_SUB* type_sub;
  } uv;
};

SPerl_TYPE_EXPR* SPerl_TYPE_EXPR_new(SPerl_PARSER* parser);

#endif
