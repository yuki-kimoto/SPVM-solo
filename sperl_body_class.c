#include "sperl_body_class.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_BODY_CLASS* SPerl_BODY_CLASS_new(SPerl_PARSER* parser) {
  SPerl_BODY_CLASS* body_class = SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_BODY_CLASS));
  
  body_class->op_subs = SPerl_ALLOCATOR_new_array(parser, 0);
  
  return body_class;
}
