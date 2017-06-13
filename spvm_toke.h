#ifndef SPVM_TOKE_H
#define SPVM_TOKE_H

#include "spvm_.h"

int SPVM_yylex(SPVM_YYSTYPE* yylvalp, SPVM_PARSER* parser);
SPVM_OP* SPVM_TOKE_newOP(SPVM_PARSER* parser, int32_t type);

#endif
