#ifndef SPERL_SPERLY_H
#define SPERL_SPERLY_H

union SPerl_yystype
{
  SPerl_OP* opval;
};

#define YYSTYPE SPerl_YYSTYPE
#define YYPRINT(file, type, value) SPerl_yyprint(file, type, value)

extern int SPerl_yydebug;

int SPerl_yyparse(SPerl_PARSER* parser);
void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s);
void SPerl_yyerror_format(SPerl_PARSER* parser, SPerl_char* message, ...);

#endif
