#include "sperl_word.h"
#include "sperl_parser.h"

SPerl_WORD* SPerl_WORD_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_WORD));
}
