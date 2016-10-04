#include <stdlib.h>

#include "sperl_my_var_info.h"
#include "sperl_allocator.h"
#include "sperl_parser.h"

SPerl_MY_VAR_INFO* SPerl_MY_VAR_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_MY_VAR_INFO*)SPerl_ALLOCATOR_alloc(parser->allocator, sizeof(SPerl_MY_VAR_INFO));
}
