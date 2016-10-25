#include <stdlib.h>

#include "sperl_type.h"

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  return (SPerl_TYPE*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE));
}
