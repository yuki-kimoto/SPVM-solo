#ifndef SPERL_PARSER_H
#define SPERL_PARSER_H

#include "sperl_op.h"

/* Parser information */
typedef struct {
  /* Source data */
  char* linestr;

  /* Current buffer position */
  char* bufptr;
  
  /* Expect next */
  int8_t expect;
  
  SPerl_OP* main_root;
} SPerl_yy_parser;

typedef union
{
  SPerl_OP* opval;
  int8_t ival;
} SPerl_yystype;

void SPerl_yyerror(SPerl_yy_parser* parser, const char* s);

/* Expected token */
enum SPerl_OP_EXPECT {
  SPerl_OP_EXPECT_NORMAL,
  SPerl_OP_EXPECT_WORD
};

#define YYSTYPE SPerl_yystype

#endif
