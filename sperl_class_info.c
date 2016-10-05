#include <stdlib.h>
#include <stdio.h>

#include "sperl_class_info.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new(SPerl_PARSER* parser) {
  SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_CLASS_INFO));
  
  return class_info;
}
