#include <stdlib.h>

#include "sperl_field_info.h"
#include "sperl_allocator.h"
#include "sperl_parser.h"

SPerl_FIELD_INFO* SPerl_FIELD_INFO_new(SPerl_PARSER* parser) {
  return (SPerl_FIELD_INFO*)SPerl_ALLOCATOR_alloc(parser->allocator, sizeof(SPerl_FIELD_INFO));
}
