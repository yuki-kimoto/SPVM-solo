#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_resolved_type.h"
#include "sperl_package.h"
#include "sperl_type_component_name.h"
#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_allocator.h"
#include "sperl_yacc_util.h"
#include "sperl_array.h"
#include "sperl_bytecode_array.h"
#include "sperl_sub.h"

SPerl_PARSER* SPerl_PARSER_new(SPerl* sperl) {
  SPerl_PARSER* parser = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_PARSER));
  
  // Parser information
  parser->sub_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->op_packages = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->package_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->op_types = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->op_use_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->field_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->include_pathes = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->bufptr = "";
  parser->resolved_types = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->resolved_type_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->use_package_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->cur_op_cases = SPerl_ALLOCATOR_new_array(sperl, 0);
  parser->constant_utf8_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  parser->cur_line = 0;
  
  // Core types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    SPerl_OP* op_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(sperl);
    SPerl_ARRAY_push(resolved_type->part_names, name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
  }
  
  // Core array types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_ARRAY_NAMES[i];
    const char* core_name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(sperl);
    SPerl_ARRAY_push(resolved_type->part_names, core_name);
    SPerl_ARRAY_push(resolved_type->part_names, "[");
    SPerl_ARRAY_push(resolved_type->part_names, "]");
    
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = SPerl_RESOLVED_TYPE_C_CORE_LENGTH + i;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
  }
  
  return parser;
}

int32_t SPerl_PARSER_parse(SPerl* sperl, const char* package_name) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  /* Build use information */
  SPerl_OP* op_package_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, package_name, 1);
  op_package_name->uv.name = package_name;
  
  // Use OP
  SPerl_OP* op_use = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_USE, package_name, 1);
  SPerl_OP_sibling_splice(sperl, op_use, NULL, 0, op_package_name);
  
  /* Push package use information stack */
  SPerl_ARRAY_push(parser->op_use_stack, op_use);
  
  // Entry point
  char* entry_point = SPerl_ALLOCATOR_new_string(sperl, strlen(package_name) + 6);
  strncpy(entry_point, package_name, strlen(package_name));
  strncpy(entry_point + strlen(package_name), "::main", 6);
  parser->entry_point = entry_point;
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  int32_t parse_success = SPerl_yyparse(sperl);
  
  return parse_success;
}

