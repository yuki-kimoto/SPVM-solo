#include <string.h>

#include "sperl_const_value.h"
#include "sperl_parser.h"
#include "sperl_hash.h"

SPerl_CONST_VALUE* SPerl_CONST_VALUE_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_CONST_VALUE));
}

SPerl_CONST_VALUE* SPerl_CONST_VALUE_create_int_1(SPerl_PARSER* parser) {
  SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
  
  const_value->code = SPerl_CONST_VALUE_C_CODE_INT;
  const_value->uv.int_value = 1;
  const_value->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
  
  return const_value;
}
