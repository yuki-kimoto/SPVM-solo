#include <stdlib.h>

#include "sperl_subtype.h"

SPerl_SUBTYPE* SPerl_SUBTYPE_new(SPerl_PARSER* parser) {
  return (SPerl_SUBTYPE*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_SUBTYPE));
}
