#include <stdlib.h>

#include "sperl_my_var.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_MY_VAR* SPerl_MY_VAR_new(SPerl_PARSER* parser) {
  return (SPerl_MY_VAR*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_MY_VAR));
}
