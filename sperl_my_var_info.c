#include <stdlib.h>

#include "sperl_my_var_info.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_MY_VAR_INFO* SPerl_MY_VAR_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_MY_VAR_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_MY_VAR_INFO));
}
