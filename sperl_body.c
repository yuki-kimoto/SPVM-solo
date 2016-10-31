#include <stdlib.h>
#include <stdio.h>

#include "sperl_body.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_char* const SPerl_BODY_C_CODE_NAMES[] = {
  "class",
  "enum",
  "core"
};

SPerl_BODY* SPerl_BODY_new(SPerl_PARSER* parser) {
  SPerl_BODY* body = (SPerl_BODY*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_BODY));
  
  return body;
}
