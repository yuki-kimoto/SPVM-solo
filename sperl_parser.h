#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_array.h"
#include "sperl_hash.h"

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
  
  /* method information array */
  SPerl_ARRAY* method_infos;
  
  /* method information hash */
  SPerl_HASH* method_info_h;
  
  SPerl_int line;
};

union SPerl_yystype_
{
  SPerl_OP* opval;
  SPerl_int ival;
};

void SPerl_yyerror(SPerl_yy_parser* parser, const SPerl_char* s);

SPerl_yy_parser* SPerl_new_parser();
void SPerl_PARSER_dump_method_infos(SPerl_yy_parser* parser);

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

#define YYSTYPE SPerl_yystype

#endif
