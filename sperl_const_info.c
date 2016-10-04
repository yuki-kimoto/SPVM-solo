#include <stdlib.h>

#include "sperl_const_info.h"
#include "sperl_allocator.h"
#include "sperl_parser.h"

SPerl_CONST_INFO* SPerl_CONST_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_CONST_INFO*)SPerl_ALLOCATOR_alloc(parser->allocator, sizeof(SPerl_CONST_INFO));
}
