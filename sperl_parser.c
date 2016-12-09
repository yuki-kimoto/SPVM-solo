#include <stdlib.h>

#include "sperl_parser.h"
#include "sperl_resolved_type.h"
#include "sperl_package.h"
#include "sperl_body_core.h"
#include "sperl_body.h"
#include "sperl_type_component_word.h"
#include "sperl_type.h"
#include "sperl_word.h"
#include "sperl_op.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_allocator.h"

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory. This is freed last.
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  // Parser information
  parser->op_subs = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->sub_complete_name_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->op_packages = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->package_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->op_types = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->op_use_stack = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->bodys = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->body_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->field_complete_name_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->enum_complete_name_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->include_pathes = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->bufptr = "";
  parser->resolved_types = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->resolved_type_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);

  // Core types
  for (SPerl_int i = 0; i < SPerl_BODY_CORE_C_CODE_LENGTH; i++) {
    // Name
    SPerl_char* name = SPerl_BODY_CORE_C_CODE_NAMES[i];
    
    // Body core
    SPerl_BODY_CORE* body_core = SPerl_BODY_CORE_new(parser);
    body_core->code = i;
    body_core->size = SPerl_BODY_CORE_C_SIZES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    SPerl_WORD* name_word = SPerl_WORD_new(parser);
    name_word->value = name;
    op_name->uv.word = name_word;
    
    // Body
    SPerl_BODY* body = SPerl_BODY_new(parser);
    body->code = SPerl_BODY_C_CODE_CORE;
    body->op_name = op_name;
    body->uv.body_core = body_core;
    
    SPerl_ARRAY_push(parser->bodys, body);
    SPerl_HASH_insert(parser->body_symtable, name, strlen(name), body);
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(parser);
    resolved_type->code = SPerl_RESOLVED_TYPE_C_CODE_CORE;
    SPerl_ARRAY_push(resolved_type->part_names, name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    resolved_type->body = body;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
    
    // Type word
    SPerl_TYPE_COMPONENT_WORD* type_component_word = SPerl_TYPE_COMPONENT_WORD_new(parser);
    type_component_word->op_name = op_name;
    
    // Type
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_WORD;
    type->uv.type_component_word = type_component_word;
    type->resolved_type = resolved_type;
    type->resolved = 1;
    
    // Type OP
    SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, NULL, NULL);
    op_type->uv.type = type;
    
    // Add type
    SPerl_ARRAY_push(parser->op_types, op_type);
    
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    SPerl_OP* op_package_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    op_package_name->uv.word = name_word;
    package->op_name = op_package_name;
    package->op_type = op_type;
    
    // Package OP
    SPerl_OP* op_package = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_PACKAGE, NULL, NULL);
    op_package->uv.package = package;
    
    SPerl_ARRAY_push(parser->op_packages, op_package);
    SPerl_HASH_insert(parser->package_symtable, name, strlen(name), type);
  }
  
  return parser;
}

void* SPerl_ALLOCATOR_alloc_memory_pool(SPerl_PARSER* parser, SPerl_int size) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, size);
}

void SPerl_PARSER_free(SPerl_PARSER* parser) {

  // Free all array pointers
  for (SPerl_int i = 0; i < parser->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(parser->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(parser->array_ptrs);
  
  // Free all hash pointers
  for (SPerl_int i = 0; i < parser->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(parser->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(parser->hash_ptrs);
  
  // Free all string pointers;
  for (SPerl_int i = 0; i < parser->long_str_ptrs->length; i++) {
    SPerl_char* str = SPerl_ARRAY_fetch(parser->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(parser->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(parser->memory_pool);
  
  free(parser);
}
