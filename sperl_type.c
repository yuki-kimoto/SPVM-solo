#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_type_component_word.h"
#include "sperl_type_component_array.h"
#include "sperl_type_component_sub.h"
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
  
  return type;
}

SPerl_boolean SPerl_TYPE_is_core_type_name(SPerl_PARSER* parser, SPerl_TYPE* type) {
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    SPerl_TYPE_COMPONENT_WORD* type_component_word = type->uv.type_component_word;
    SPerl_WORD* name_word = type_component_word->name_word;
    SPerl_char* name = name_word->value;
    
    if (strcmp(name, "boolean") == 0 || strcmp(name, "char") == 0 || strcmp(name, "byte") == 0 || strcmp(name, "short") == 0
      || strcmp(name, "int") == 0 || strcmp(name, "long") == 0 || strcmp(name, "float") == 0
      || strcmp(name, "double") == 0) 
    {
      return 1;
    }
  }
  else {
    return 0;
  }
}

SPerl_boolean SPerl_TYPE_is_core_type(SPerl_PARSER* parser, SPerl_int type_id) {
  // type_id is set after calling resolve_types
  if (type_id >= 0 && type_id < 8) {
    return 1;
  }
  else {
    return 0;
  }
}

SPerl_TYPE* SPerl_TYPE_create_word_type(SPerl_PARSER* parser, SPerl_char* type_name) {

  SPerl_WORD* type_name_word = SPerl_WORD_new(parser);
  type_name_word->value = type_name;
  
  SPerl_TYPE_COMPONENT_WORD* type_component_word = SPerl_TYPE_COMPONENT_WORD_new(parser);
  type_component_word->name_word = type_name_word;
  
  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_WORD;
  type->uv.type_component_word = type_component_word;
  
  return type;
}

SPerl_TYPE* SPerl_TYPE_create_array_type(SPerl_PARSER* parser, SPerl_char* type_name) {

  SPerl_WORD* type_name_word = SPerl_WORD_new(parser);
  type_name_word->value = type_name;
  
  SPerl_TYPE_COMPONENT_WORD* type_component_word = SPerl_TYPE_COMPONENT_WORD_new(parser);
  type_component_word->name_word = type_name_word;
  
  SPerl_TYPE* type_w = SPerl_TYPE_new(parser);
  type_w->code = SPerl_TYPE_C_CODE_WORD;
  type_w->uv.type_component_word = type_component_word;
  
  SPerl_TYPE_COMPONENT_ARRAY* type_component_array = SPerl_TYPE_COMPONENT_ARRAY_new(parser);
  type_component_array->type = type_w;

  SPerl_TYPE* type = SPerl_TYPE_new(parser);
  type->code = SPerl_TYPE_C_CODE_ARRAY;
  type->uv.type_component_array = type_component_array;
  
  return type;
}

void SPerl_TYPE_to_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts) {
  
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    SPerl_TYPE_PART* part = SPerl_TYPE_PART_new(parser);
    part->code = SPerl_TYPE_PART_C_CODE_WORD;
    part->uv.name_word = type->uv.type_component_word->name_word;
    SPerl_ARRAY_push(parts, part);
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array = type->uv.type_component_array;
    
    SPerl_TYPE_to_parts(parser, type_component_array->type, parts);
    
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
    SPerl_TYPE_COMPONENT_SUB* type_component_sub = type->uv.type_component_sub;
    SPerl_ARRAY* argument_types = type_component_sub->argument_types;
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
    SPerl_TYPE* return_type = type_component_sub->return_type;
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

void SPerl_TYPE_build_name(SPerl_PARSER* parser, SPerl_TYPE* type) {
  
  if (type->name) {
    return;
  }
  else {
    SPerl_ARRAY* parts = type->parts;
    SPerl_int total_length = 0;

    for (SPerl_int i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      SPerl_char code = part->code;
      
      if (code == SPerl_TYPE_PART_C_CODE_CHAR) {
        total_length += 1;
      }
      else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
        total_length += strlen(part->uv.name_word->value);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
        total_length += 3;
      }
    }
    
    SPerl_char* type_name = SPerl_PARSER_new_string(parser, total_length);
    
    SPerl_int pos = 0;
    for (SPerl_int i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      SPerl_char code = part->code;
      
      SPerl_int length = 0;
      if (code == SPerl_TYPE_PART_C_CODE_CHAR) {
        length = 1;
        memcpy(type_name + pos, part->uv.char_name, length);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
        length = strlen(part->uv.name_word->value);
        memcpy(type_name + pos, part->uv.name_word->value, length);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
        length = 3;
        memcpy(type_name + pos, "sub", length);
      }
      pos += length;
    }
    
    type->name = type_name;
  }
}
