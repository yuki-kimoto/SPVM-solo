#include <stdlib.h>

#include "sperl_type.h"

SPerl_char* const SPerl_TYPE_C_NAMES[] = {
  "core",
  "class_or_typedef",
  "class",
  "typedef",
  "array",
  "sub"
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  return (SPerl_TYPE*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE));
}
