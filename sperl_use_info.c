#include <stdlib.h>
#include <stdio.h>

#include "sperl_use_info.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_USE_INFO* SPerl_USE_INFO_new(SPerl_PARSER* parser) {
  SPerl_USE_INFO* use_info = (SPerl_USE_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_USE_INFO));
  
  return use_info;
}
