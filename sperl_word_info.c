#include <stdlib.h>
#include <stdio.h>

#include "sperl_word_info.h"
#include "sperl_parser.h"
#include "sperl_memory_pool.h"

SPerl_WORD_INFO* SPerl_WORD_INFO_new(SPerl_PARSER* parser) {
  SPerl_WORD_INFO* word_info = (SPerl_WORD_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_WORD_INFO));
  
  return word_info;
}
