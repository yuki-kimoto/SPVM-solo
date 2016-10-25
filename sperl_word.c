#include <stdlib.h>
#include <stdio.h>

#include "sperl_word.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_WORD* SPerl_WORD_new(SPerl_PARSER* parser) {
  SPerl_WORD* word = (SPerl_WORD*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_WORD));
  
  return word;
}
