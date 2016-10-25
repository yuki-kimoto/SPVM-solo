#include <stdlib.h>
#include <stdio.h>

#include "sperl_class.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_CLASS* SPerl_CLASS_new(SPerl_PARSER* parser) {
  SPerl_CLASS* class = (SPerl_CLASS*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_CLASS));
  
  return class;
}
