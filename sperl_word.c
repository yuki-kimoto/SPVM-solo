#include "sperl_word.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_WORD* SPerl_WORD_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_WORD));
}
