%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "sperl_type.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
  #include "sperl_var_info.h"
  #include "sperl_my_var_info.h"

  static void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval) {
    
    SPerl_VAR_INFO* var_info;
    switch(type) {
      case MULOP:
        fprintf(file, "%d", yylval.ival);
        break;
      case WORD:
        fprintf(file, "\"%s\"", ((SPerl_OP*)yylval.opval)->uv.string_value);
        break;
      case VAR:
        var_info = (SPerl_VAR_INFO*)((SPerl_OP*)yylval.opval)->uv.ptr_value;
        fprintf(file, "\"%s\"", var_info->name);
        break;
    }
  }
%}

%token <ival> '+' '-'
%token <ival> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <ival> RELOP
%token <ival> LAST NEXT
%token <opval> WORD VAR CONST

%type <opval> grammar statements statement declmy declhas if else block
%type <opval> optterms terms term subargs subarg optsubargs
%type <opval> desctype descripters descripter
%type <opval> type pkgname fieldname subname package packages

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc RELOP
%left <ival> SHIFTOP
%left <ival> ADDOP
%left <ival> MULOP
%right <ival> NOTOP '~' UMINUS
%nonassoc <ival> INCOP DECOP
%left <ival> ARROW
%nonassoc <ival> ')'
%left <ival> '('
%left '[' '{'

%%

grammar
  : /* NULL */
    {
      $$ = SPerl_OP_newOP_GRAMMER(parser, NULL);
    }
  | packages
    {
      $$ = SPerl_OP_newOP_GRAMMER(parser, $1);;
    }

packages
  : packages package
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | package

package
  : PACKAGE pkgname block
    {
      $$ = SPerl_OP_newOP_PACKAGE(parser, $2, $3, NULL);
    }
  | PACKAGE pkgname ':' descripters block
    {
      $$ = SPerl_OP_newOP_PACKAGE(parser, $2, $5, $4);
    }

statements
  : statements statement 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | statement

statement
  : SUB subname '(' optsubargs ')' ':' desctype block
    {
      $$ = SPerl_OP_newOP_SUB(parser, $2, $4, $7, $8);
    }
  | FOR '(' term ';' term ';' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_LOOP,
        $3,
        $5
      );
      SPerl_OP_sibling_splice(parser, op, $5, 0, $9);
      SPerl_OP_sibling_splice(parser, op, $9, 0, $7);
      $$ = op;
    }
  | WHILE '(' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_LOOP,
        SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL),
        $3
      );
      SPerl_OP_sibling_splice(parser, op, $3, 0, $5);
      $$ = op;
    }
  | term ';'
  | ';'
    {
      $$ = (SPerl_OP*)NULL;
    }
  | if
  | LAST ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_LAST, NULL, NULL);
    }
  | NEXT ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_NEXT, NULL, NULL);
    }
  | RETURN term ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_RETURN, $2, NULL);
    }
  | USE pkgname ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_USE, $2, NULL);
    }
  | USE pkgname '-' WORD';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_USE, $2, $4);
    }
  | block

if
  : IF '(' term ')' block else
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_COND, $3, $5);
      if ($6) {
        SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      }
      $$ = op;
    }

else
  : /* NULL */
    {
      $$ = (SPerl_OP*)0;
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block else
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_COND, $3, $5);
      if ($6) {
        SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      }
      $$ = op;
    }

declmy
  : MY VAR ':' desctype
    {
      $$ = SPerl_OP_newOP_MY(parser, $2, $4);
    }

declhas
  : HAS fieldname ':' desctype
    {
      $$ = SPerl_OP_newOP_HAS(parser, $2, $4);
    }

optterms
  :	/* NULL */
    {
      $$ = (SPerl_OP*)NULL;
    }
  |	terms

terms
  : terms ',' term
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | term

term
  : VAR
  | CONST
    {
      $$ = SPerl_OP_newOP_CONST(parser, $1);
    }
  | '+' term %prec UMINUS
    {
      $$ = $2;
    }
  | INCOP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_PREINC, $2, NULL);
    }
  | term INCOP
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_POSTINC, $1, NULL);
    }
  | DECOP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_PREDEC, $2, NULL);
    }
  | term DECOP
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_POSTDEC, $1, NULL);
    }
  | NOTOP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_NOT, $2, NULL);
    }
  | '~' term
    {
      $$ = SPerl_OP_newOP(parser, $1, $2, NULL);
    }
  | '-' term %prec UMINUS
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_NEGATE, $2, NULL);
    }
  | term '+' term %prec ADDOP
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ADD, $1, $3);
    }
  | term '-' term %prec ADDOP
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_SUBTRACT, $1, $3);
    }
  | term MULOP term
    {
      $$ = SPerl_OP_newOP(parser, $2, $1, $3);
    }
  | term RELOP term
    {
      $$ = SPerl_OP_newOP(parser, $2, $1, $3);
    }
  | term BITANDOP term
    {
      $$ = SPerl_OP_newOP(parser, $2, $1, $3);
    }
  | term BITOROP term
    {
      $$ = SPerl_OP_newOP(parser, $2, $1, $3);
    }
  | term SHIFTOP term
    {
      $$ = SPerl_OP_newOP(parser, $2, $1, $3);
    }
  | VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_AELEM, $1, $4);
    }
  | VAR ARROW fieldname
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_FIELD, $1, $3);
    }
  | subname '(' optterms  ')'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_CALLSUB, $1, $3);
    }
  | term ASSIGNOP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ASSIGN, $1, $3);
    }
  | term ANDOP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_AND, $1, $3);
    }
  | term OROP term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_OR, $1, $3);
    }
  | VAR ARROW subname '(' optterms ')'
    {
      $$ = SPerl_OP_newOP(
        parser,
        SPerl_OP_CALLSUB,
        $3,
        SPerl_OP_append_elem(parser, $1, $5)
      );
    }
  | VAR ARROW '(' optterms ')'
    {
      $$ = SPerl_OP_newOP(
        parser,
        SPerl_OP_CALLSUB,
        $1,
        $4
      );
    }
  | pkgname ARROW subname '(' optterms ')'
    {
      $$ = SPerl_OP_newOP(
        parser,
        SPerl_OP_CALLSUB,
        SPerl_OP_append_elem(parser, $1, $3),
        $5
      );
    }
  | declmy
  | declhas

block 
  : '{' '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_BLOCK, NULL, NULL);
    }
  | '{' statements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_BLOCK, $2, NULL);
    }

optsubargs
  :	/* NULL */
    {
      $$ = (SPerl_OP*)NULL;
    }
  |	subargs

subargs
  : subargs ',' subarg
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | subarg

subarg
  : VAR ':' desctype
    {
      $$ = SPerl_OP_newOP_MY(parser, $1, $3);
    }

desctype
  : type
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_DESCTYPE, $1, 0);
    }
  | descripters type
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_DESCTYPE, $2, $1);
    }
    
type : WORD

descripters
  : descripters descripter
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | descripter

descripter : WORD
fieldname : WORD
subname : WORD
pkgname : WORD

%%

