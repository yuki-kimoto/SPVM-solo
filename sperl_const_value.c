#include "sperl_const_value.h"
#include "sperl_parser.h"

SPerl_CONST_VALUE* SPerl_CONST_VALUE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_CONST_VALUE));
}

SPerl_CONST_VALUE* SPerl_CONST_VALUE_copy(SPerl_PARSER* parser, SPerl_CONST_VALUE* const_value) {
  SPerl_CONST_VALUE* new_const_value = SPerl_CONST_VALUE_new(parser);
  new_const_value->code = const_value->code;
  new_const_value->uv = const_value->uv;
  
  new_const_value->type = const_value->type;
  new_const_value->op = const_value->op;
  
  return new_const_value;
}
