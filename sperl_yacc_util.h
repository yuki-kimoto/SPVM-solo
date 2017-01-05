#ifndef SPERL_YACC_H
#define SPERL_YACC_H

#include "sperl_base.h"
#include "stdio.h"

union SPerl_yystype
{
  SPerl_OP* opval;
};

#define YYSTYPE SPerl_YYSTYPE
#define YYPRINT(file, type, value) SPerl_yyprint(file, type, value)

extern int SPerl_yydebug;

int SPerl_yyparse(SPerl* sperl);
void SPerl_yyerror(SPerl* sperl, const char* s);
void SPerl_yyerror_format(SPerl* sperl, const char* message, ...);
void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval);

#endif
