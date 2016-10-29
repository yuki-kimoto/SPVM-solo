#include <stdlib.h>
#include <stdio.h>

#include "sperl_body_enum.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_BODY_ENUM* SPerl_BODY_ENUM_new(SPerl_PARSER* parser) {
  SPerl_BODY_ENUM* body_enum = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_BODY_ENUM));
  
  return body_enum;
}
