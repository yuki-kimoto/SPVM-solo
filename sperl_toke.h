#ifndef SPERL_TOKE_H
#define SPERL_TOKE_H

#include "sperl.h"

int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl* sperl);
SPerl_OP* SPerl_TOKE_newOP(SPerl* sperl, int32_t type);

#endif
