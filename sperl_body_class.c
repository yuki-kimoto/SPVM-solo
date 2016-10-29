#include <stdlib.h>
#include <stdio.h>

#include "sperl_body_class.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser) {
  SPerl_BODY_CLASS* body_class = (SPerl_BODY_CLASS*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_BODY_CLASS));
  
  return body_class;
}
