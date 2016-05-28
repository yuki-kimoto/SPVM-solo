%pure-parser
%parse-param	{ yy_parser* parser }
%lex-param	{ yy_parser* parser }

%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "sperl.h"
%}

%token <ival> MY OUR HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR
%token <ival> RELOP EQOP
%token <ival> LAST CONTINUE
%token <opval> WORD VAR INT

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc EQOP
%nonassoc RELOP
/* %left <ival> SHIFTOP */
%left ADDOP /* "+" or "-" */
%left MULOP
/* %right <ival> '!' '~' UMINUS */
/* %nonassoc <ival> PREINC PREDEC POSTINC POSTDEC */
/* %left <ival> ARROW */
%nonassoc <ival> ')'
%left <ival> '('
/* %left '[' '{' */

%%

grammar
  : /* NULL */
    { printf("NULL -> grammar\n"); }
  | statements
    { printf("statements -> grammar\n"); }

statements
  : statement
    { printf("statement -> statements\n"); }
  | statements statement 
    { printf("statements statement -> statements\n"); }

block 
  : '{' '}'
    { printf("{ } -> block\n"); }
  | '{' statements '}'
    { printf("{ statements } -> block\n"); }

statement
  : SUB subname subdefargslist block
    { printf("sub subname subdefargslist block -> statement\n"); }
  | term ';'
    { printf("term ; -> statement\n") }
  | ';'
    { printf("; -> statement\n") }
  | if
    { printf("if -> statement\n"); }
  | declaration
  | FOR '(' declaration term ';' term ')' '{' statements '}'
    { printf("FOR ( declaration term ; term ) { statements }\n"); }
  | PACKAGE WORD ';'
    { printf("PACKAGE WORD ; -> statement\n"); }
  | LAST ';'
    { printf("LAST ; -> statement\n"); }
  | CONTINUE ';'
    { printf("CONTINUE ; -> statement\n"); }
  
declaration
  : declword declvar ';'
    { printf("declword declvar ; -> declaration\n"); }
  | declword declvar ASSIGNOP term ';'
    { printf("declword declvar = term ; -> declaration\n"); }
  | declword declvar ':' type ';'
    { printf("declword declvar : type ; -> declaration\n"); }
  | declword declvar ':' type ASSIGNOP term ';'
    { printf("declword declvar : type ASSIGNOP term ; -> declaration\n"); }

declword
  : MY
    { printf("MY -> declword\n"); }
  | OUR
    { printf("OUR -> declword\n"); }
  | HAS
    { printf("HAS -> declword\n"); }

declvar
  : VAR
    { printf("VAR -> declvar\n"); } /* my, our */
  | WORD
    { printf("VAR -> declvar\n"); } /* has */

if
  : IF '(' term ')' block
    { printf("if ( term ) block\n"); }
  | if else
    { printf("if else -> if\n"); }

else
  : ELSE block
    { printf("else block"); }
  | ELSIF '(' term ')' block else
    { printf("elsif ( term ) block else\n"); }

terms
  : term
    { printf("term -> terms\n"); }
  | terms ',' term
    { printf("terms , term -> terms\n"); }

term
  : VAR
    { printf("VAR -> term (%s)\n", ((SVOP*)$1)->uv.pv); }
  | term ADDOP term
    { printf("term + term -> term\n"); }
  | term MULOP term
    { printf("term * term -> term\n"); }
  | INT
    { printf("INT -> term (%d)\n", ((SVOP*)$1)->uv.iv); }
  | subname '(' ')'
    { printf("subname () -> term\n"); }
  | subname '(' terms  ')'
    { printf("subname (terms) -> term\n"); }
  | RETURN term
    { printf("RETURN term -> term\n"); }
  | RETURN list
    { printf("RETURN list -> term\n"); }
  | term RELOP term
    { printf("term RELOP term%d\n", $2); }
  | term EQOP term
    { printf("term EQOP term\n"); }
  | term ASSIGNOP term
    { printf("term ASSIGNOP term\n"); }
  | term ANDOP term
    { printf("term ANDOP term -> term\n"); }
  | term OROP term
    { printf("term OROP term -> term\n"); }
  | term BITANDOP term
    { printf("term BITANDOP term -> term\n"); }
  | term BITOROP term
    { printf("term BITOROP term -> term\n"); }
  | SUB subdefargslist block
    { printf("SUB subdefargslist block -> term\n"); }
    
subname
  : WORD
    { printf("WORD -> subname\n"); }

list
  :  '(' ')'
    { printf("( ) -> list\n"); }
  | '(' terms ')'
    { printf("( terms ) -> list\n"); }

subdefargslist
  : '(' ')'
    { printf("( ) -> subdefargslist\n"); }
  | '(' subdefargs ')'
    { printf("( subdefargs ) -> subdefargslist\n"); }

subdefargs
  : subdefarg
    { printf("subdefarg -> subdefargs\n"); }
  | subdefargs ',' subdefarg
    { printf("subdefargs , subdefarg\n"); }

subdefarg
  : VAR ':' type
    { printf("VAR : type -> subdefarg (%s)\n", ((SVOP*)$1)->uv.pv); }

type
  : WORD
    { printf("WORD -> type (%s)\n", ((SVOP*)$1)->uv.pv); }

%%

