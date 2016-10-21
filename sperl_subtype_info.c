#include <stdlib.h>

#include "sperl_subtype_info.h"

SPerl_SUBTYPE_INFO* SPerl_SUBTYPE_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_SUBTYPE_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_SUBTYPE_INFO));
}
