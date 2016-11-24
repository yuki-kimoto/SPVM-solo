#include <string.h>

#include "sperl_constant.h"
#include "sperl_parser.h"
#include "sperl_hash.h"

SPerl_CONSTANT* SPerl_CONSTANT_new(SPerl_PARSER* parser) {
  return SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_CONSTANT));
}

SPerl_CONSTANT* SPerl_CONSTANT_create_int_1(SPerl_PARSER* parser) {
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
  
  constant->code = SPerl_CONSTANT_C_CODE_INT;
  constant->uv.int_value = 1;
  constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
  
  return constant;
}