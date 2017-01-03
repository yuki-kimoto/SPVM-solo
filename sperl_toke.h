#ifndef SPERL_TOKE_H
#define SPERL_TOKE_H

#include "sperl.h"

// Expected token
enum {
  SPerl_TOKE_C_EXPECT_NORMAL,
  SPerl_TOKE_C_EXPECT_NAME,
  SPerl_TOKE_C_EXPECT_PACKAGENAME
};

int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl* sperl);
SPerl_OP* SPerl_TOKE_newOP(SPerl* sperl, uint8_t type);

#endif
