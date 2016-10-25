#include <stdlib.h>

#include "sperl_descripter.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_DESCRIPTER* SPerl_DESCRIPTER_new(SPerl_PARSER* parser) {
  return (SPerl_DESCRIPTER*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_DESCRIPTER));
}

SPerl_char* const SPerl_DESCRIPTER_NAMES[] = {
  "const",
  "static",
  "value",
  "enum"
};
