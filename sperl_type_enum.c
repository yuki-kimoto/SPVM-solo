#include <stdlib.h>
#include <stdio.h>

#include "sperl_type_enum.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_TYPE_ENUM* SPerl_TYPE_ENUM_new(SPerl_PARSER* parser) {
  SPerl_TYPE_ENUM* type_enum = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_ENUM));
  
  return type_enum;
}
