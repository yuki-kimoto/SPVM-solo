#include <stdlib.h>

#include "sperl_field_info.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_FIELD_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_FIELD_INFO));
}
