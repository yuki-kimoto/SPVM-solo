#include <stdlib.h>
#include <stdio.h>

#include "sperl_core_data.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_char* const SPerl_CORE_DATA_C_CODE_NAMES[] = {
  "boolean",
  "char",
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double"
};

SPerl_CORE_DATA* SPerl_CORE_DATA_new(SPerl_PARSER* parser) {
  SPerl_CORE_DATA* core_data = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_CORE_DATA));
  
  return core_data;
}
