#include <stdlib.h>

#include "sperl_type_word.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_TYPE_WORD* SPerl_TYPE_WORD_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE_WORD));
}
