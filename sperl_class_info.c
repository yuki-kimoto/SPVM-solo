#include <stdlib.h>
#include <stdio.h>

#include "sperl_class_info.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"

SPerl_CLASS_INFO* SPerl_CLASS_INFO_new(SPerl_PARSER* parser) {
  SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)SPerl_ALLOCATOR_alloc(parser->allocator, sizeof(SPerl_CLASS_INFO));
  
  return class_info;
}
