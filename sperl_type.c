#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_array.h"
#include "sperl_type_part.h"
#include "sperl_op.h"
#include "sperl_allocator_parser.h"
#include "sperl_hash.h"
#include "sperl_resolved_type.h"
#include "sperl_yacc_util.h"
#include "sperl_package.h"

const char* const SPerl_TYPE_C_CODE_NAMES[] = {
  "name",
  "array",
};

// Resolve type and index type
_Bool SPerl_TYPE_resolve_type(SPerl* sperl, SPerl_OP* op_type, int32_t name_length) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_HASH* op_package_symtable = parser->op_package_symtable;
  SPerl_TYPE* type = op_type->uv.type;
  
  if (type->resolved_type) {
    return 1;
  }
  else {
    SPerl_ARRAY* resolved_type_part_names = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
    
    SPerl_ARRAY* parts = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser, 0);
    SPerl_TYPE_build_parts(sperl, type, parts);
    
    for (int32_t i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(sperl, parts, i);
      if (part->code == SPerl_TYPE_PART_C_CODE_SUB) {
        name_length += 3;
        SPerl_ARRAY_push(sperl, resolved_type_part_names, "sub");
      }
      else if (part->code == SPerl_TYPE_PART_C_CODE_BYTE) {
        name_length++;
        SPerl_ARRAY_push(sperl, resolved_type_part_names, (void*)part->uv.char_name);
      }
      else {
        const char* part_name = part->uv.op_name->uv.name;
        
        // Core type
        if (strcmp(part_name, "boolean") == 0 || strcmp(part_name, "byte") == 0 || strcmp(part_name, "short") == 0 || strcmp(part_name, "int") == 0
          || strcmp(part_name, "long") == 0 || strcmp(part_name, "float") == 0 || strcmp(part_name, "double") == 0)
        {
          SPerl_ARRAY_push(sperl, resolved_type_part_names, (void*)part_name);
        }
        else {
          // Package
          SPerl_OP* op_found_package = SPerl_HASH_search(sperl, op_package_symtable, part_name, strlen(part_name));
          if (op_found_package) {
            SPerl_ARRAY_push(sperl, resolved_type_part_names, (void*)part_name);
          }
          else {
            SPerl_yyerror_format(sperl, "unknown package \"%s\" at %s line %d\n", part_name, op_type->file, op_type->line);
            return 0;
          }
        }
        name_length += strlen(part_name);
      }
    }
    char* resolved_type_name = SPerl_ALLOCATOR_PARSER_new_string(sperl, parser, name_length);
    
    int32_t cur_pos = 0;
    for (int32_t i = 0; i < resolved_type_part_names->length; i++) {
      const char* resolved_type_part_name = (const char*) SPerl_ARRAY_fetch(sperl, resolved_type_part_names, i);
      int32_t resolved_type_part_name_length = (int32_t)strlen(resolved_type_part_name);
      memcpy(resolved_type_name + cur_pos, resolved_type_part_name, resolved_type_part_name_length);
      cur_pos += resolved_type_part_name_length;
    }
    resolved_type_name[cur_pos] = '\0';
    
    // Create resolved type id
    SPerl_RESOLVED_TYPE* found_resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, resolved_type_name, strlen(resolved_type_name));
    if (found_resolved_type) {
      type->resolved_type = found_resolved_type;
    }
    else if (parser->resolved_types->length >= SPerl_OP_LIMIT_TYPES) {
      SPerl_yyerror_format(sperl, "too many resolved types, the type \"%s\" ignored at %s line %d\n", resolved_type_name, op_type->file, op_type->line);
      return 0;
    }
    else {
      SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(sperl);
      resolved_type->id = parser->resolved_types->length;
      resolved_type->name = resolved_type_name;
      SPerl_ARRAY_push(sperl, parser->resolved_types, resolved_type);
      SPerl_HASH_insert(sperl, parser->resolved_type_symtable, resolved_type_name, strlen(resolved_type_name), resolved_type);
      type->resolved_type = resolved_type;
    }
  }
  
  return 1;
}

SPerl_TYPE* SPerl_TYPE_new(SPerl* sperl) {
  SPerl_TYPE* type = SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_TYPE));
  
  return type;
}

void SPerl_TYPE_build_parts(SPerl* sperl, SPerl_TYPE* type, SPerl_ARRAY* parts) {
  
  if (type->code == SPerl_TYPE_C_CODE_NAME) {
    SPerl_TYPE_PART* part = SPerl_TYPE_PART_new(sperl);
    part->code = SPerl_TYPE_PART_C_CODE_NAME;
    part->uv.op_name = type->uv.type_component_name->op_name;
    SPerl_ARRAY_push(sperl, parts, part);
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array = type->uv.type_component_array;
    
    SPerl_TYPE_build_parts(sperl, type_component_array->type, parts);
    
    SPerl_TYPE_PART* type_part_openbracket = SPerl_TYPE_PART_new(sperl);
    type_part_openbracket->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_openbracket->uv.char_name = "[";
    SPerl_ARRAY_push(sperl, parts, type_part_openbracket);
    
    SPerl_TYPE_PART* type_part_closebracket = SPerl_TYPE_PART_new(sperl);
    type_part_closebracket->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_closebracket->uv.char_name = "]";
    SPerl_ARRAY_push(sperl, parts, type_part_closebracket);
  }
}

