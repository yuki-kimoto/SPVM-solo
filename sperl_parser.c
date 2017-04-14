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
#include "sperl_allocator_util.h"
#include "sperl_allocator_parser.h"
#include "sperl_yacc_util.h"
#include "sperl_array.h"
#include "sperl_bytecode_array.h"
#include "sperl_sub.h"

SPerl_PARSER* SPerl_PARSER_new(SPerl* sperl) {
  SPerl_PARSER* parser = malloc(sizeof(SPerl_PARSER));

  // Allocator
  parser->allocator = SPerl_ALLOCATOR_PARSER_new(sperl);
  
  sperl->parser = parser;
  
  // Parser information
  parser->op_sub_symtable = SPerl_ALLOCATOR_PARSER_new_hash(sperl, parser->allocator, 0);
  parser->op_packages = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->op_package_symtable = SPerl_ALLOCATOR_PARSER_new_hash(sperl, parser->allocator, 0);
  parser->op_types = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->op_use_symtable = SPerl_ALLOCATOR_PARSER_new_hash(sperl, parser->allocator, 0);
  parser->op_use_stack = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->op_field_symtable = SPerl_ALLOCATOR_PARSER_new_hash(sperl, parser->allocator, 0);
  parser->include_pathes = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->bufptr = "";
  parser->resolved_types = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->resolved_type_symtable = SPerl_ALLOCATOR_PARSER_new_hash(sperl, parser->allocator, 0);
  parser->cur_op_cases = SPerl_ALLOCATOR_PARSER_new_array(sperl, parser->allocator, 0);
  parser->cur_line = 0;
  
  // Core types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    SPerl_OP* op_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(sperl);
    SPerl_ARRAY_push(sperl, resolved_type->part_names, (void*)name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    SPerl_ARRAY_push(sperl, parser->resolved_types, resolved_type);
    SPerl_HASH_insert(sperl, parser->resolved_type_symtable, name, strlen(name), resolved_type);
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
    SPerl_ARRAY_push(sperl, resolved_type->part_names, (void*)core_name);
    SPerl_ARRAY_push(sperl, resolved_type->part_names, "[");
    SPerl_ARRAY_push(sperl, resolved_type->part_names, "]");
    
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = SPerl_RESOLVED_TYPE_C_CORE_LENGTH + i;
    SPerl_ARRAY_push(sperl, parser->resolved_types, resolved_type);
    SPerl_HASH_insert(sperl, parser->resolved_type_symtable, name, strlen(name), resolved_type);
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
  SPerl_ARRAY_push(sperl, parser->op_use_stack, op_use);
  
  // Entry point
  int32_t package_name_length = (int32_t)strlen(package_name);
  int32_t entry_point_sub_name_length =  (int32_t)(package_name_length + 6);
  char* entry_point_sub_name = SPerl_ALLOCATOR_UTIL_safe_malloc_i32(entry_point_sub_name_length + 1, sizeof(char));
  strncpy(entry_point_sub_name, package_name, package_name_length);
  strncpy(entry_point_sub_name + package_name_length, "::main", 6);
  entry_point_sub_name[entry_point_sub_name_length] = '\0';
  sperl->entry_point_sub_name = entry_point_sub_name;
  
  // use standard module
  SPerl_OP* op_use_std = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_USE, "std", 0);
  SPerl_OP* op_std_package_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "std", 0);
  op_std_package_name->uv.name = "std";
  SPerl_OP_sibling_splice(sperl, op_use_std, NULL, 0, op_std_package_name);
  SPerl_ARRAY_push(sperl, parser->op_use_stack, op_use_std);
  SPerl_HASH_insert(sperl, parser->op_use_symtable, op_std_package_name->uv.name, strlen(op_std_package_name->uv.name), op_use_std);
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  int32_t parse_success = SPerl_yyparse(sperl);
  
  return parse_success;
}

void SPerl_PARSER_free(SPerl* sperl, SPerl_PARSER* parser) {
  
  // Free allocator
  SPerl_ALLOCATOR_PARSER_free(sperl, parser->allocator);
  
  free(parser);
}
