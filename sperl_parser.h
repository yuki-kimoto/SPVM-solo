#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include <stdio.h>

#include "sperl_base.h"

// Parser information
struct SPerl_yy_parser_{
  // Before buffer position
  SPerl_char* befbufptr;
  
  // Current buffer position
  SPerl_char* bufptr;
  
  // Expect next token type
  SPerl_int expect;

  // Current file name
  SPerl_char* cur_file;
  
  // Source data
  SPerl_char* cur_src;
  
  // Current line number
  SPerl_int cur_line;
  
  // Syntax error count
  SPerl_int error_count;
  
  // AST grammer
  SPerl_OP* op_grammer;
  
  // Packages
  SPerl_ARRAY* op_packages;
  SPerl_HASH* package_symtable;
  
  // Bodys
  SPerl_ARRAY* bodys;
  SPerl_HASH* body_symtable;
  
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
  SPerl_ARRAY* op_subs;
  
  // Include pathes
  SPerl_ARRAY* include_pathes;
  
  // Current package count
  SPerl_int current_package_count;
  
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
  
  SPerl_boolean fatal_error;
};

SPerl_PARSER* SPerl_PARSER_new();

void SPerl_PARSER_free(SPerl_PARSER* parser);

#endif
