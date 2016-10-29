#ifndef SPERL_TYPE_EXPR_SUB_H
#define SPERL_TYPE_EXPR_SUB_H

#include "sperl_base.h"

struct SPerl_type_expr_sub {
  SPerl_ARRAY* argument_type_exprs;
  SPerl_TYPE_EXPR* return_type_expr;
};

SPerl_TYPE_EXPR_SUB* SPerl_TYPE_EXPR_SUB_new(SPerl_PARSER* parser);

#endif
