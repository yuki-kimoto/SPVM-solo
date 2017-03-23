#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include <stdio.h>

#include "sperl_base.h"

// Parser information
struct SPerl_parser {
  // Memory_pool - This is compile time memory pool. This memory pool save short string and object except array, hash
  SPerl_MEMORY_POOL* memory_pool;
  
  // Compile time arrays
  SPerl_ARRAY* arrays;
  
  // Compile time hashes
  SPerl_ARRAY* hashes;
  
  // Compile time long strings
  SPerl_ARRAY* long_strings;

  // Before buffer position
  const char* befbufptr;
  
  // Current buffer position
  const char* bufptr;
  
  // Current file name
  const char* cur_module_path;
  
  // Source data
  const char* cur_src;

  // AST grammar
  SPerl_OP* op_grammar;
  
  // Packages
  SPerl_ARRAY* op_packages;
  
  // OP package symtable
  SPerl_HASH* op_package_symtable;
  
  // Class loading stack
  SPerl_ARRAY* op_use_stack;
  
  // Include pathes
  SPerl_ARRAY* include_pathes;
  
  // Method absolute name symbol table
  SPerl_HASH* op_sub_symtable;
  
  // Field absolute name symbol table
  SPerl_HASH* op_field_symtable;
  
  // use symbol table
  SPerl_HASH* op_use_symtable;
  
  // Types
  SPerl_ARRAY* op_types;
  
  // Resovled types
  SPerl_ARRAY* resolved_types;
  
  // Resolved type string symbol table
  SPerl_HASH* resolved_type_symtable;

  // Current case statements in switch statement
  SPerl_ARRAY* cur_op_cases;
  
  // Entry point subroutine name
  const char* start_sub_name;

  // Current line number
  int64_t cur_line;
  
  // Syntax error count
  int64_t error_count;
  
  // Current package count
  int64_t current_package_count;
  
  // Error is fatal
  _Bool fatal_error;
};

SPerl_PARSER* SPerl_PARSER_new(SPerl* sperl);
int64_t SPerl_PARSER_parse(SPerl* sperl, const char* package_name);

void SPerl_PARSER_free(SPerl* sperl, SPerl_PARSER* parser);

#endif
