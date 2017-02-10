#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include <stdio.h>

#include "sperl_base.h"

// Parser information
struct SPerl_parser {
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
  SPerl_HASH* package_symtable;
  
  // Class loading stack
  SPerl_ARRAY* op_use_stack;

  // Current op subs
  SPerl_ARRAY* current_op_subs;
  
  // Include pathes
  SPerl_ARRAY* include_pathes;

  // Method absolute name symbol table
  SPerl_HASH* sub_symtable;
  
  // Field absolute name symbol table
  SPerl_HASH* field_symtable;
  
  // use symbol table
  SPerl_HASH* use_package_symtable;
  
  // Constant string table
  SPerl_HASH* constant_utf8_symtable;
  
  // Types
  SPerl_ARRAY* op_types;
  
  // Resovled types
  SPerl_ARRAY* resolved_types;
  
  // Resolved type string symbol table
  SPerl_HASH* resolved_type_symtable;

  // Current case statements in switch statement
  SPerl_ARRAY* cur_op_cases;
  
  // Entry point subroutine
  const char* entry_point;

  // Current line number
  int32_t cur_line;
  
  // Syntax error count
  int32_t error_count;
  
  // Current package count
  int32_t current_package_count;
  
  // Error is fatal
  _Bool fatal_error;
};

SPerl_PARSER* SPerl_PARSER_new(SPerl* sperl);
int32_t SPerl_PARSER_parse(SPerl* sperl, const char* package_name);

#endif
