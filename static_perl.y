%pure-parser
%parse-param	{ yy_parser* parser }
%lex-param	{ yy_parser* parser }

%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "static_perl.h"
%}

%token <ival> '+' '-'
%token <ival> MY OUR HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <ival> RELOP EQOP
%token <ival> LAST CONTINUE
%token <opval> WORD VAR INT STRING BOOL

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc EQOP
%nonassoc RELOP
%left <ival> SHIFTOP
%left ADDOP
%left MULOP
%right <ival> NOTOP BITNOTOP UMINUS
%nonassoc <ival> INCOP DECOP
%left <ival> ARROW
%nonassoc <ival> ')'
%left <ival> '('
%left '[' '{'

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
  : SUB subname '(' ')' block
    { printf("SUB subname ( ) block -> statement\n"); }
  | SUB subname '(' subdefargs ')' block
    { printf("SUB subname ( subdefargs ) block -> statement\n"); }
  | OUR SUB subname '(' ')' block
    { printf("OUR SUB subname ( ) block -> statement\n"); }
  | OUR SUB subname '(' subdefargs ')' block
    { printf("OUR SUB subname ( subdefargs ) block -> statement\n"); }
  | term ';'
    { printf("term ; -> statement\n") }
  | ';'
    { printf("; -> statement\n") }
  | if
    { printf("if -> statement\n"); }
  | declaration
    { printf("declaration -> statement\n"); }
  | FOR '(' declaration term ';' term ')' '{' statements '}'
    { printf("FOR ( declaration term ; term ) { statements }\n"); }
  | WHILE '(' term ')' block
    { printf("WHILE ( term ) block\n"); }
  | PACKAGE WORD ';'
    { printf("PACKAGE WORD ; -> statement\n"); }
  | LAST ';'
    { printf("LAST ; -> statement\n"); }
  | CONTINUE ';'
    { printf("CONTINUE ; -> statement\n"); }
  | RETURN term ';'
    { printf("RETURN term ; -> statement\n"); }
  | RETURN '(' terms ')' ';'
    { printf("RETURN ( terms ) ; -> statement\n"); }
  | USE WORD ';'
    { printf("USE WORD ; -> statement\n"); }
  | USE WORD '(' ')' ';'
    { printf("USE WORD ( ) ; -> statement\n"); }
  | USE WORD '(' words ')' ';'
    { printf("USE WORD ( words ) ; -> statement\n"); }
  | STRING
    { printf("STRING(%s) -> statement\n", ((SVOP_STATIC*)$1)->uv.pv); }
    
words
  : WORD
  | words ',' WORD

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
    { printf("VAR -> term (%s)\n", ((SVOP_STATIC*)$1)->uv.pv); }
  | NOTOP term
    { printf("NOTOP term -> term\n"); }
  | BITNOTOP term
    { printf("BITNOTOP term -> term\n"); }
  | INCOP term
    { printf("INCOP term\n") }
  | term INCOP
    { printf("term INCOP\n") }
  | DECOP term
    { printf("DECOP term\n") }
  | term DECOP
    { printf("term DECOP\n") }
  | '+' term %prec UMINUS
    { printf("+ term -> term\n"); }
  | '-' term %prec UMINUS
    { printf("- term -> term\n"); }
  | term '+' term %prec ADDOP
    { printf("term + term -> term\n"); }
  | term '-' term %prec ADDOP
    { printf("term - term -> term\n"); }
  | term MULOP term
    { printf("term * term -> term\n"); }
  | INT
    { printf("INT -> term (%d)\n", ((SVOP_STATIC*)$1)->uv.iv); }
  | BOOL
    { printf("BOOL -> term (%d)\n", ((SVOP_STATIC*)$1)->uv.iv); }
  | subname '(' ')'
    { printf("subname () -> term\n"); }
  | subname '(' terms  ')'
    { printf("subname (terms) -> term\n"); }
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
  | term SHIFTOP term
    { printf("term SHIFTOP term\n"); }
  | SUB '(' ')' block
    { printf("SUB () block -> term\n"); }
  | SUB '(' subdefargs ')' block
    { printf("SUB ( subdefargs ) block -> term\n"); }
  | '(' term ')'
    { printf("( term ) -> term\n"); }
  | VAR ARROW WORD
    { printf("VAR ARROW WORD -> term\n"); }
  | VAR ARROW WORD ASSIGNOP term
    { printf("VAR ARROW WORD ASSIGNOP term -> term\n"); }
  | VAR ARROW WORD '(' ')'
    { printf("VAR ARROW WORD ( )\n"); }
  | VAR ARROW WORD '(' terms ')'
    { printf("VAR ARROW WORD ( terms )\n"); }
  | WORD ARROW WORD
    { printf("VAR ARROW WORD -> term\n"); }
  | WORD ARROW WORD ASSIGNOP term
    { printf("VAR ARROW WORD ASSIGNOP term -> term\n"); }
  | WORD ARROW WORD '(' ')'
    { printf("VAR ARROW WORD ( )\n"); }
  | WORD ARROW WORD '(' terms ')'
    { printf("VAR ARROW WORD ( terms )\n"); }
  | VAR ARROW '[' term ']'
    { printf("VAR ARROW [ term ]\n"); }

subname
  : WORD
    { printf("WORD -> subname\n"); }

subdefargs
  : subdefarg
    { printf("subdefarg -> subdefargs\n"); }
  | subdefargs ',' subdefarg
    { printf("subdefargs , subdefarg\n"); }

subdefarg
  : VAR ':' type
    { printf("VAR : type -> subdefarg (%s)\n", ((SVOP_STATIC*)$1)->uv.pv); }

type
  : WORD
    { printf("WORD -> type (%s)\n", ((SVOP_STATIC*)$1)->uv.pv); }

%%

