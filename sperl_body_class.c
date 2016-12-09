#include "sperl_body_class.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser) {
  return SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_BODY_CLASS));
}
