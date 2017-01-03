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
#include "sperl_yacc.h"
#include "sperl_array.h"
#include "sperl_use.h"

SPerl* SPerl_new() {
  SPerl* sperl = calloc(1, sizeof(SPerl));
  
  sperl->parser = SPerl_PARSER_new(sperl);
  
  // Manipulate memory. This is freed last.
  sperl->array_ptrs = SPerl_ARRAY_new(0);
  sperl->hash_ptrs = SPerl_ARRAY_new(0);
  sperl->long_str_ptrs = SPerl_ARRAY_new(0);
  sperl->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  // Parser information
  sperl->cur_op_subs = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->sub_abs_name_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  sperl->op_packages = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->package_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  sperl->op_types = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->op_use_stack = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->field_abs_name_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  sperl->include_pathes = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->bufptr = "";
  sperl->resolved_types = SPerl_ALLOCATOR_new_array(sperl, 0);
  sperl->resolved_type_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  sperl->use_package_symtable = SPerl_ALLOCATOR_new_hash(sperl, 0);
  sperl->cur_op_cases = SPerl_ALLOCATOR_new_array(sperl, 0);
  
  // Core types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "CORE", 1);
    
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(sperl);
    SPerl_ARRAY_push(resolved_type->part_names, (void*) name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    SPerl_ARRAY_push(sperl->resolved_types, resolved_type);
    SPerl_HASH_insert(sperl->resolved_type_symtable, name, strlen(name), resolved_type);
    
    // Type name
    SPerl_TYPE_COMPONENT_NAME* type_component_name = SPerl_TYPE_COMPONENT_NAME_new(sperl);
    type_component_name->op_name = op_name;
    
    // Type
    SPerl_TYPE* type = SPerl_TYPE_new(sperl);
    type->code = SPerl_TYPE_C_CODE_NAME;
    type->uv.type_component_name = type_component_name;
    type->resolved_type = resolved_type;
    
    // Type OP
    SPerl_OP* op_type = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_TYPE, "CORE", 1);
    op_type->uv.type = type;
    
    // Add type
    SPerl_ARRAY_push(sperl->op_types, op_type);
    
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(sperl);
    package->size = SPerl_RESOLVED_TYPE_C_CORE_SIZES[i];
    SPerl_OP* op_package_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_package_name->uv.name = name;
    package->op_name = op_package_name;
    package->op_type = op_type;
    
    // Package OP
    SPerl_OP* op_package = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_DECL_PACKAGE, "CORE", 1);
    op_package->uv.package = package;
    
    SPerl_ARRAY_push(sperl->op_packages, op_package);
    SPerl_HASH_insert(sperl->package_symtable, name, strlen(name), type);
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
    SPerl_ARRAY_push(resolved_type->part_names, (void*) core_name);
    SPerl_ARRAY_push(resolved_type->part_names, (void*) "[");
    SPerl_ARRAY_push(resolved_type->part_names, (void*) "]");
    
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = SPerl_RESOLVED_TYPE_C_CORE_LENGTH + i;
    SPerl_ARRAY_push(sperl->resolved_types, resolved_type);
    SPerl_HASH_insert(sperl->resolved_type_symtable, name, strlen(name), resolved_type);
  }
  
  return sperl;
}

int32_t SPerl_parse(SPerl* sperl, const char* package_name) {

  /* Build use information */
  SPerl_USE* use = SPerl_USE_new(sperl);
  SPerl_OP* op_package_name = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_NAME, package_name, 1);
  op_package_name->uv.name = package_name;
  use->op_package_name = op_package_name;
  
  // Use OP
  SPerl_OP* op_use = SPerl_OP_newOP(sperl, SPerl_OP_C_CODE_USE, package_name, 1);
  op_use->uv.use = use;
  
  /* Push package use information stack */
  SPerl_ARRAY_push(sperl->op_use_stack, op_use);
  
  // Entry point
  char* entry_point = SPerl_ALLOCATOR_new_string(sperl, strlen(package_name) + 6);
  strncpy(entry_point, package_name, strlen(package_name));
  strncpy(entry_point + strlen(package_name), "::main", 6);
  sperl->entry_point = entry_point;
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  int32_t parse_success = SPerl_yyparse(sperl);
  
  return parse_success;
}

void SPerl_free(SPerl* sperl) {

  // Free all array pointers
  for (int32_t i = 0; i < sperl->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(sperl->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(sperl->array_ptrs);
  
  // Free all hash pointers
  for (int32_t i = 0; i < sperl->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(sperl->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(sperl->hash_ptrs);
  
  // Free all string pointers;
  for (int32_t i = 0; i < sperl->long_str_ptrs->length; i++) {
    void* str = SPerl_ARRAY_fetch(sperl->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(sperl->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(sperl->memory_pool);
  
  free(sperl);
}
