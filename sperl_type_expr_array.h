#ifndef SPERL_TYPE_EXPR_ARRAY_H
#define SPERL_TYPE_EXPR_ARRAY_H

#include "sperl_base.h"

struct SPerl_type_expr_array {
  SPerl_TYPE_EXPR* type_expr;
};

SPerl_TYPE_EXPR_ARRAY* SPerl_TYPE_EXPR_ARRAY_new(SPerl_PARSER* parser);

#endif
