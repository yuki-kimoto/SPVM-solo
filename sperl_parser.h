#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_const_info.h"

/* Parser information */
struct SPerl_yy_parser_{
  /* Source data */
  SPerl_char* linestr;

  /* Current buffer position */
  SPerl_char* bufptr;
  
  /* Expect next */
  SPerl_int expect;
  
  /* AST root */
  SPerl_OP* main_root;
  
  /* class information array */
  SPerl_ARRAY* class_infos;
  
  /* class information hash */
  SPerl_HASH* class_info_h;
  
  /* current method information array */
  SPerl_ARRAY* current_method_infos;
  
  /* current constant op infirmation array */
  SPerl_ARRAY* current_const_ops;

  /* current constant op infirmation array */
  SPerl_ARRAY* current_const_infos;
  
  /* constant pool */
  SPerl_int* const_pool_number;
  SPerl_char* const_pool_string;
  
  /* method information hash */
  SPerl_HASH* method_info_h;
  
  SPerl_int current_const_pool_size;
  
  SPerl_int line;
};

union SPerl_yystype_
{
  SPerl_OP* opval;
  SPerl_int ival;
};

void SPerl_yyerror(SPerl_yy_parser* parser, const SPerl_char* s);

SPerl_yy_parser* SPerl_new_parser();
void SPerl_PARSER_dump_method_info(SPerl_METHOD_INFO* method_info);
void SPerl_PARSER_dump_class_infos(SPerl_yy_parser* parser);
void SPerl_PARSER_dump_const_op(SPerl_OP* const_op);
void SPerl_PARSER_dump_const_info(SPerl_CONST_INFO* const_info);
void SPerl_PARSER_dump_const_pool(SPerl_char* const_pool, SPerl_CONST_INFO* const_info);

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

#define YYSTYPE SPerl_yystype

#endif
