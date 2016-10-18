#include <stdlib.h>

#include "sperl_descripter_info.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_DESCRIPTER_INFO* SPerl_DESCRIPTER_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_DESCRIPTER_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_DESCRIPTER_INFO));
}

SPerl_char* const SPerl_DESCRIPTER_INFO_type_names[] = {
  "const",
  "static",
  "value",
  "enum"
};
