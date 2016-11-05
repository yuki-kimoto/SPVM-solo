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
  SPerl_ARRAY* packages;
  SPerl_HASH* package_symtable;
  
  // Bodys
  SPerl_ARRAY* bodys;
  SPerl_HASH* body_symtable;
  
  // Constant informations
  SPerl_ARRAY* const_values;
  
  // Constant pool
  SPerl_int* const_pool;
  
  // Constant pool capacity
  SPerl_int const_pool_capacity;
  
  // Constant pool length
  SPerl_int const_pool_pos;
  
  // Memory_pool
  SPerl_MEMORY_POOL* memory_pool;
  
  // All array pointers
  SPerl_ARRAY* array_ptrs;
  
  // All hash pointers
  SPerl_ARRAY* hash_ptrs;
  
  // All long string pointers
  SPerl_ARRAY* long_str_ptrs;
  
  // Class loading stack
  SPerl_ARRAY* use_stack;
  
  // Variable id
  SPerl_int next_var_id;
  
  // Current methods(named sub + anon sub) in class 
  SPerl_ARRAY* current_methods;
  
  // Include pathes
  SPerl_ARRAY* include_pathes;
  
  // Current package count
  SPerl_int current_package_count;
  
  // Current use class name
  SPerl_char* current_use_package_name;
  
  // Current type index
  SPerl_int current_type_id;
  
  // Resolved type string symbol table
  SPerl_HASH* type_resolved_string_symtable;
  
  // Method global name symbol table(Class name, method name, argument count)
  SPerl_HASH* method_complete_name_symtable;
  
  // Call subroutines
  SPerl_ARRAY* callsubs;
};

SPerl_PARSER* SPerl_PARSER_new();
void SPerl_PARSER_dump_parser(SPerl_PARSER* parser);
void SPerl_PARSER_dump_classs(SPerl_PARSER* parser, SPerl_ARRAY* classs);
void SPerl_PARSER_dump_const_values(SPerl_PARSER* parser, SPerl_ARRAY* const_values);
void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int size);
void SPerl_PARSER_dump_const_value(SPerl_PARSER* parser, SPerl_CONST_VALUE* const_value);
void SPerl_PARSER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field);
void SPerl_PARSER_dump_method(SPerl_PARSER* parser, SPerl_METHOD* method);
void SPerl_PARSER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var);
void SPerl_PARSER_dump_enum_value(SPerl_PARSER* parser, SPerl_ENUM_VALUE* enum_value);
void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* types);
void SPerl_PARSER_dump_bodys(SPerl_PARSER* parser, SPerl_ARRAY* bodys);
void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* packages);

void SPerl_PARSER_free(SPerl_PARSER* parser);
SPerl_ARRAY* SPerl_PARSER_new_array(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_HASH* SPerl_PARSER_new_hash(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length);
SPerl_int* SPerl_PARSER_new_int(SPerl_PARSER* parser);

void* SPerl_PARSER_alloc_memory_pool(SPerl_PARSER* parser, SPerl_int size);

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth);

#endif
