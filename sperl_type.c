#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_type_word.h"
#include "sperl_type_array.h"
#include "sperl_type_sub.h"
#include "sperl_parser.h"
#include "sperl_word.h"
#include "sperl_array.h"
#include "sperl_type_part.h"

SPerl_char* const SPerl_TYPE_C_CODE_NAMES[] = {
  "word",
  "array",
  "sub",
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  SPerl_TYPE* type = SPerl_PARSER_alloc_memory_pool(parser, sizeof(SPerl_TYPE));
  
  type->parts = SPerl_PARSER_new_array(parser, 0);
  type->resolved_part_names = SPerl_PARSER_new_array(parser, 0);
  
  return type;
}

void SPerl_TYPE_to_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts) {
  
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    SPerl_TYPE_PART* part = SPerl_TYPE_PART_new(parser);
    part->code = SPerl_TYPE_PART_C_CODE_WORD;
    part->uv.name_word = type->uv.type_word->name_word;
    SPerl_ARRAY_push(parts, part);
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_TYPE_ARRAY* type_array = type->uv.type_array;
    
    SPerl_TYPE_to_parts(parser, type_array->type, parts);
    
    SPerl_TYPE_PART* type_part_openbracket = SPerl_TYPE_PART_new(parser);
    type_part_openbracket->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_openbracket->uv.char_name = "[";
    SPerl_ARRAY_push(parts, type_part_openbracket);
    
    SPerl_TYPE_PART* type_part_closebracket = SPerl_TYPE_PART_new(parser);
    type_part_closebracket->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_closebracket->uv.char_name = "]";
    SPerl_ARRAY_push(parts, type_part_closebracket);
  }
  else if (type->code == SPerl_TYPE_C_CODE_SUB) {
    // (
    SPerl_TYPE_PART* type_part_openparen1 = SPerl_TYPE_PART_new(parser);
    type_part_openparen1->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_openparen1->uv.char_name = "(";
    SPerl_ARRAY_push(parts, type_part_openparen1);
    
    // sub
    SPerl_TYPE_PART* type_part_sub = SPerl_TYPE_PART_new(parser);
    type_part_sub->code = SPerl_TYPE_PART_C_CODE_SUB;
    SPerl_ARRAY_push(parts, type_part_sub);
    
    // (
    SPerl_TYPE_PART* type_part_openparen2 = SPerl_TYPE_PART_new(parser);
    type_part_openparen2->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_openparen2->uv.char_name = "(";
    SPerl_ARRAY_push(parts, type_part_openparen2);
    
    // Argument types
    SPerl_TYPE_SUB* type_sub = type->uv.type_sub;
    SPerl_ARRAY* argument_types = type_sub->argument_types;
    for (SPerl_int i = 0; i < argument_types->length; i++) {
      SPerl_TYPE* argument_type = SPerl_ARRAY_fetch(argument_types, i);
      SPerl_TYPE_to_parts(parser, argument_type, parts);
      if (i != argument_types->length - 1) {
        SPerl_TYPE_PART* type_part_comma = SPerl_TYPE_PART_new(parser);
        type_part_comma->code = SPerl_TYPE_PART_C_CODE_CHAR;
        type_part_comma->uv.char_name = ",";
        SPerl_ARRAY_push(parts, type_part_comma);
      }
    }
    
    // )
    SPerl_TYPE_PART* type_part_closeparen1 = SPerl_TYPE_PART_new(parser);
    type_part_closeparen1->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_closeparen1->uv.char_name = ")";
    SPerl_ARRAY_push(parts, type_part_closeparen1);
    
    // Return type
    SPerl_TYPE* return_type = type_sub->return_type;
    SPerl_TYPE_to_parts(parser, return_type, parts);
    
    // )
    SPerl_TYPE_PART* type_part_closeparen2 = SPerl_TYPE_PART_new(parser);
    type_part_closeparen2->code = SPerl_TYPE_PART_C_CODE_CHAR;
    type_part_closeparen2->uv.char_name = ")";
    SPerl_ARRAY_push(parts, type_part_closeparen2);
  }
}

void SPerl_TYPE_build_parts(SPerl_PARSER* parser, SPerl_TYPE* type) {
  SPerl_ARRAY* parts = SPerl_PARSER_new_array(parser, 0);
  SPerl_TYPE_to_parts(parser, type, parts);
  type->parts = parts;
}

void SPerl_TYPE_print(SPerl_PARSER* parser, SPerl_TYPE* type, FILE* fh) {
  SPerl_ARRAY* parts = type->parts;
  for (SPerl_int i = 0; i < parts->length; i++) {
    SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
    SPerl_char code = part->code;
    
    if (code == SPerl_TYPE_PART_C_CODE_CHAR) {
      fprintf(fh, "%s", part->uv.char_name);
    }
    else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
      fprintf(fh, "%s", part->uv.name_word->value);
    }
    else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
      fprintf(fh, "sub");
    }
  }
}

