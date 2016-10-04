#include <stdlib.h>

#include "sperl_var_info.h"

SPerl_VAR_INFO* SPerl_VAR_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_VAR_INFO*)SPerl_ALLOCATOR_alloc(parser->allocator, sizeof(SPerl_VAR_INFO));
}
