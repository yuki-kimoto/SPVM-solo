#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include <stdio.h>

#include "sperl_type.h"

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
  
  // class informations
  SPerl_ARRAY* class_infos;
  
  // class information symbol table
  SPerl_HASH* class_info_symtable;
  
  // Constant informations
  SPerl_ARRAY* const_infos;
  
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
  SPerl_ARRAY* class_stack;
  
  // Variable id
  SPerl_int next_var_id;
};

SPerl_PARSER* SPerl_PARSER_new();
void SPerl_PARSER_dump_parser_info(SPerl_PARSER* parser);
void SPerl_PARSER_dump_class_infos(SPerl_PARSER* parser, SPerl_ARRAY* class_infos);
void SPerl_PARSER_dump_const_infos(SPerl_PARSER* parser, SPerl_ARRAY* const_infos);
void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int size);
void SPerl_PARSER_dump_const_info(SPerl_PARSER* parser, SPerl_CONST_INFO* const_info);
void SPerl_PARSER_dump_field_info(SPerl_PARSER* parser, SPerl_FIELD_INFO* field_info);
void SPerl_PARSER_dump_method_info(SPerl_PARSER* parser, SPerl_METHOD_INFO* method_info);
void SPerl_PARSER_dump_my_var_info(SPerl_PARSER* parser, SPerl_MY_VAR_INFO* my_var_info);

void SPerl_PARSER_free(SPerl_PARSER* parser);
SPerl_ARRAY* SPerl_PARSER_new_array(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_HASH* SPerl_PARSER_new_hash(SPerl_PARSER* parser, SPerl_int capacity);
SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length);

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth);

/* sperl_toke.c */
// Expected token
enum SPerl_op_expect {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl_PARSER* parser);

/* sperly.y(sperly.tab.c) */
union SPerl_yystype
{
  SPerl_OP* opval;
  SPerl_int ival;
};

#define YYSTYPE SPerl_YYSTYPE
#define YYPRINT(file, type, value) SPerl_yyprint(file, type, value)

extern int SPerl_yydebug;

int SPerl_yyparse(SPerl_PARSER* parser);
void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s);

#endif
