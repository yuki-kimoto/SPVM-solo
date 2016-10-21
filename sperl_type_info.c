#include <stdlib.h>

#include "sperl_type_info.h"

SPerl_TYPE_INFO* SPerl_TYPE_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_TYPE_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_INFO));
}
