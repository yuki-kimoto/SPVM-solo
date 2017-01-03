#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include <stdio.h>

#include "sperl_base.h"

// Parser information
struct SPerl_yy_sperl_{
  // Before buffer position
  const char* befbufptr;
  
  // Current buffer position
  const char* bufptr;
  
  // Expect next token type
  int32_t expect;

  // Current file name
  const char* cur_module_path;
  
  // Source data
  const char* cur_src;
  
  // Current line number
  int32_t cur_line;
  
  // Syntax error count
  int32_t error_count;
  
  // AST grammar
  SPerl_OP* op_grammar;
  
  // Packages
  SPerl_ARRAY* op_packages;
  SPerl_HASH* package_symtable;
  
  // Memory_pool
  SPerl_MEMORY_POOL* memory_pool;
  
  // All array pointers
  SPerl_ARRAY* array_ptrs;
  
  // All hash pointers
  SPerl_ARRAY* hash_ptrs;
  
  // All long string pointers
  SPerl_ARRAY* long_str_ptrs;
  
  // Class loading stack
  SPerl_ARRAY* op_use_stack;

  // Current subs(named sub + anon sub) in class 
  SPerl_ARRAY* cur_op_subs;
  
  // Include pathes
  SPerl_ARRAY* include_pathes;
  
  // Current package count
  int32_t current_package_count;
  
  // Method absolute name symbol table
  SPerl_HASH* sub_abs_name_symtable;
  
  // Field absolute name symbol table
  SPerl_HASH* field_abs_name_symtable;
  
  // use symbol table
  SPerl_HASH* use_package_symtable;
  
  // Types
  SPerl_ARRAY* op_types;
  
  // Resovled types
  SPerl_ARRAY* resolved_types;
  
  // Resolved type string symbol table
  SPerl_HASH* resolved_type_symtable;
  
  // Error is fatal
  _Bool fatal_error;
  
  // Current case statements in switch statement
  SPerl_ARRAY* cur_op_cases;
  
  // Entry point subroutine
  const char* entry_point;
};

SPerl* SPerl_new();

void SPerl_free(SPerl* sperl);
int32_t SPerl_parse(SPerl* sperl, const char* package_name);

#endif
