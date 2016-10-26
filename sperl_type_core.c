#include <stdlib.h>
#include <stdio.h>

#include "sperl_type_core.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_TYPE_CORE* SPerl_TYPE_CORE_new(SPerl_PARSER* parser) {
  SPerl_TYPE_CORE* type_core = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_CORE));
  
  return type_core;
}
