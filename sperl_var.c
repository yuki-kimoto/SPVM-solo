#include <stdlib.h>

#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_VAR* SPerl_VAR_new(SPerl_PARSER* parser) {
  return (SPerl_VAR*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_VAR));
}
