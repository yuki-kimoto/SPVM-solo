#include <stdlib.h>

#include "sperl_var.h"

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser) {
  return (SPerl_VAR*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_VAR));
}
