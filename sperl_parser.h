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
  
  // Current use class name
  SPerl_char* current_use_package_name;
  
  // Method global name symbol table(package name, sub name, argument count)
  SPerl_HASH* sub_complete_name_symtable;
  
  // Field complete name symbol table(package name, field_name)
  SPerl_HASH* field_complete_name_symtable;
  
  // Enum complete name symbol table(package name, enum_name)
  SPerl_HASH* enum_complete_name_symtable;
  
  // Types
  SPerl_ARRAY* op_types;
  
  // Resovled types
  SPerl_ARRAY* resolved_types;

  // Resolved type string symbol table
  SPerl_HASH* resolved_type_symtable;
  
  SPerl_boolean fatal_error;
};

SPerl_PARSER* SPerl_PARSER_new();
void SPerl_PARSER_dump_parser(SPerl_PARSER* parser);
void SPerl_PARSER_dump_classs(SPerl_PARSER* parser, SPerl_ARRAY* classs);
void SPerl_PARSER_dump_constants(SPerl_PARSER* parser, SPerl_ARRAY* constants);
void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int size);
void SPerl_PARSER_dump_constant(SPerl_PARSER* parser, SPerl_CONSTANT* constant);
void SPerl_PARSER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field);
void SPerl_PARSER_dump_sub(SPerl_PARSER* parser, SPerl_SUB* sub);
void SPerl_PARSER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var);
void SPerl_PARSER_dump_enumeration_value(SPerl_PARSER* parser, SPerl_ENUMERATION_VALUE* enumeration_value);
void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* types);
void SPerl_PARSER_dump_bodys(SPerl_PARSER* parser, SPerl_ARRAY* bodys);
void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* packages);
void SPerl_PARSER_dump_resolved_types(SPerl_PARSER* parser, SPerl_ARRAY* resolved_types);

void SPerl_PARSER_free(SPerl_PARSER* parser);
SPerl_ARRAY* SPerl_PARSER_new_array(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_HASH* SPerl_PARSER_new_hash(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length);
SPerl_int* SPerl_PARSER_new_int(SPerl_PARSER* parser);
SPerl_VMCODE* SPerl_PARSER_new_vmcode(SPerl_PARSER* parser);

void* SPerl_PARSER_alloc_memory_pool(SPerl_PARSER* parser, SPerl_int size);

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op);

#endif
