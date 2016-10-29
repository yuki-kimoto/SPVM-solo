#include <stdlib.h>
#include <stdio.h>

#include "sperl_body_core.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_char* const SPerl_BODY_CORE_C_CODE_NAMES[] = {
  "boolean",
  "char",
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double"
};

SPerl_int const SPerl_BODY_CORE_C_CODE_SIZES[] = {
  1,
  1,
  1,
  2,
  4,
  8,
  4,
  8
};

SPerl_BODY_CORE* SPerl_BODY_CORE_new(SPerl_PARSER* parser) {
  SPerl_BODY_CORE* body_core = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_BODY_CORE));
  
  return body_core;
}
