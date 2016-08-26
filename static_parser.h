#ifndef STATIC_PARSER_H
#define STATIC_PARSER_H

#include "static_op.h"

/* Parser information */
typedef struct {
  /* Source data */
  char* linestr;

  /* Current buffer position */
  char* bufptr;
  
  /* Expect next */
  int expect;
} static_yy_parser;

typedef union
{
  STATIC_OP* opval;
  int ival;
} static_yystype;

void static_yyerror(static_yy_parser* parser, const char* s);

#define YYSTYPE static_yystype

#endif
