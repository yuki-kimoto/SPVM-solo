#include <stdlib.h>

#include "sperl_var_info.h"

SPerl_VAR_INFO* SPerl_VAR_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_VAR_INFO*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_VAR_INFO));
}
