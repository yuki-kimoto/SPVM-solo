#include <stdlib.h>
#include <stdio.h>

#include "sperl_use.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_USE* SPerl_USE_new(SPerl_PARSER* parser) {
  SPerl_USE* use = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_USE));
  
  return use;
}
