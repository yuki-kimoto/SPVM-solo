#ifndef SPERL_TOKE_H
#define SPERL_TOKE_H

#include "sperl_parser.h"

// Expected token
enum {
  SPerl_TOKE_C_EXPECT_NORMAL,
  SPerl_TOKE_C_EXPECT_WORD,
  SPerl_TOKE_C_EXPECT_PACKAGENAME
};

int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl_PARSER* parser);
SPerl_OP* SPerl_TOKE_newOP(SPerl_PARSER* parser, uint8_t type);

#endif
