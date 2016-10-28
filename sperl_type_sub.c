#include <stdlib.h>

#include "sperl_type_sub.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_TYPE_SUB* SPerl_TYPE_SUB_new(SPerl_PARSER* parser) {
  return (SPerl_TYPE_SUB*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_SUB));
}
