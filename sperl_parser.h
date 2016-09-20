#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include "sperl_op.h"
#include "sperl_collection.h"

/* Parser information */
typedef struct {
  /* Source data */
  SPerl_char* linestr;

  /* Current buffer position */
  SPerl_char* bufptr;
  
  /* Expect next */
  SPerl_long expect;
  
  /* AST root */
  SPerl_OP* main_root;
  
  /* class information array */
  SPerl_ARRAY* class_infos;
  
  /* class information hash */
  SPerl_HASH* class_info_h;
  
} SPerl_yy_parser;

typedef union
{
  SPerl_OP* opval;
  SPerl_long ival;
} SPerl_yystype;

void SPerl_yyerror(SPerl_yy_parser* parser, const SPerl_char* s);

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

#define YYSTYPE SPerl_yystype

#endif
