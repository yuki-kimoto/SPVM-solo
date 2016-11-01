#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_type_word.h"
#include "sperl_type_array.h"
#include "sperl_type_sub.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_word.h"
#include "sperl_array.h"

SPerl_char* const SPerl_TYPE_C_CODE_NAMES[] = {
  "word",
  "array",
  "sub",
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE));
}

void SPerl_TYPE_to_string_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* string_parts) {
  
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    SPerl_ARRAY_push(string_parts, type->uv.type_word->name_word->value);
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_TYPE_ARRAY* type_array = type->uv.type_array;
    SPerl_TYPE_to_string_parts(parser, type_array->type, string_parts);
    
    SPerl_ARRAY_push(string_parts, "[");
    SPerl_ARRAY_push(string_parts, "]");
  }
  else if (type->code == SPerl_TYPE_C_CODE_SUB) {
    SPerl_ARRAY_push(string_parts, "(sub(");
    
    SPerl_TYPE_SUB* type_sub = type->uv.type_sub;
    SPerl_ARRAY* argument_types = type_sub->argument_types;
    for (SPerl_int i = 0; i < argument_types->length; i++) {
      SPerl_TYPE* argument_type = SPerl_ARRAY_fetch(argument_types, i);
      SPerl_TYPE_to_string_parts(parser, argument_type, string_parts);
      if (i != argument_types->length - 1) {
        SPerl_ARRAY_push(string_parts, ",");
      }
    }
    SPerl_ARRAY_push(string_parts, ")");
    SPerl_TYPE* return_type = type_sub->return_type;
    SPerl_TYPE_to_string_parts(parser, return_type, string_parts);
    SPerl_ARRAY_push(string_parts, ")");
  }
}

void SPerl_TYPE_build_string_parts(SPerl_PARSER* parser, SPerl_TYPE* type) {
  SPerl_ARRAY* string_parts = SPerl_PARSER_new_array(parser, 0);
  SPerl_TYPE_to_string_parts(parser, type, string_parts);
  type->string_parts = string_parts;
}

void SPerl_TYPE_print_string_parts(SPerl_PARSER* parser, SPerl_TYPE* type, FILE* fh) {
  SPerl_ARRAY* string_parts = type->string_parts;
  for (SPerl_int i = 0; i < string_parts->length; i++) {
    SPerl_char* string_part = SPerl_ARRAY_fetch(string_parts, i);
    fprintf(fh, string_part);
  }
}
