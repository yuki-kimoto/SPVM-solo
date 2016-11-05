#include <stdlib.h>

#include "sperl_field.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_FIELD* SPerl_FIELD_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_FIELD));
}
