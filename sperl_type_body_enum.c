#include <stdlib.h>
#include <stdio.h>

#include "sperl_type_body_enum.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_TYPE_BODY_ENUM* SPerl_TYPE_BODY_ENUM_new(SPerl_PARSER* parser) {
  SPerl_TYPE_BODY_ENUM* type_body_enum = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_BODY_ENUM));
  
  return type_body_enum;
}
