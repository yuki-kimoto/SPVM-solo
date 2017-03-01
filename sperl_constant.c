#include <string.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_constant.h"
#include "sperl_allocator.h"
#include "sperl_hash.h"

const char* const SPerl_CONSTANT_C_CODE_NAMES[] = {
  "BYTE",
  "SHORT",
  "INT",
  "LONG",
  "FLOAT",
  "DOUBLE",
  "STRING",
};

SPerl_CONSTANT* SPerl_CONSTANT_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_CONSTANT));
}

SPerl_CONSTANT* SPerl_CONSTANT_create_int_1(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
  
  constant->code = SPerl_CONSTANT_C_CODE_INT;
  constant->uv.int_value = 1;
  constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "int", strlen("int"));
  
  return constant;
}
