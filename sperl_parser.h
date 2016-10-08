#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include "sperl_type.h"
#include "stdio.h"

extern int SPerl_yydebug;

/* Parser information */
struct SPerl_yy_parser_{
  /* Current buffer position */
  SPerl_char* bufptr;
  
  /* Expect next token type */
  SPerl_int expect;

  /* Current file name */
  SPerl_char* cur_file;
  
  /* Source data */
  SPerl_char* cur_src;
  
  /* Current line number */
  SPerl_int cur_line;
  
  /* AST root */
  SPerl_OP* op_root;
  
  /* class informations */
  SPerl_ARRAY* class_infos;
  
  /* class information symbol table */
  SPerl_HASH* class_info_symtable;
  
  /* Constant informations */
  SPerl_ARRAY* const_infos;
  
  /* Constant pool */
  SPerl_int* const_pool;
  
  /* Constant pool capacity*/
  SPerl_int const_pool_capacity;
  
  /* Constant pool length */
  SPerl_int const_pool_pos;
  
  /* Memory_pool */
  SPerl_MEMORY_POOL* memory_pool;
  
  /* All array pointers */
  SPerl_ARRAY* array_ptrs;
  
  /* All hash pointers */
  SPerl_ARRAY* hash_ptrs;
  
  /* All string pointers */
  SPerl_ARRAY* str_ptrs;
  
  /* Class loading stack */
  SPerl_ARRAY* class_stack;
};

union SPerl_yystype_
{
  SPerl_OP* opval;
  SPerl_int ival;
};

void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s);

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

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth);

#define YYSTYPE SPerl_yystype
#define YYPRINT(file, type, value) SPerl_yyprint(file, type, value)

int SPerl_yylex(YYSTYPE* SPerl_yylvalp, SPerl_PARSER* parser);
int SPerl_yyparse(SPerl_PARSER* parser);

#endif
