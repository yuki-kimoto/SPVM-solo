#include <stdlib.h>

#include "sperl_enum_value_info.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_ENUM_VALUE_INFO* SPerl_ENUM_VALUE_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_ENUM_VALUE_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_ENUM_VALUE_INFO));
}
