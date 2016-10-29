#include <stdlib.h>

#include "sperl_type_expr_sub.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_TYPE_EXPR_SUB* SPerl_TYPE_EXPR_SUB_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_EXPR_SUB));
}
