#include <stdlib.h>

#include "sperl_type_expr_array.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_TYPE_EXPR_ARRAY* SPerl_TYPE_EXPR_ARRAY_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_EXPR_ARRAY));
}
