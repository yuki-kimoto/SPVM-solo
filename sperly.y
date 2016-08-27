%pure-parser
%parse-param	{ SPerl_yy_parser* parser }
%lex-param	{ SPerl_yy_parser* parser }

%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "sperl_parser.h"
  #include "sperl_op.h"
%}

%token <ival> '+' '-'
%token <ival> MY OUR HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <ival> RELOP EQOP
%token <ival> LAST CONTINUE
%token <opval> WORD VAR INT STRING BOOL

%type <opval> grammar statements block statement words declvar if else terms
%type <opval> term subname subargs subarg modiftype type modifier

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc EQOP
%nonassoc RELOP
%left <ival> SHIFTOP
%left <ival> ADDOP
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
  : SUB subname ':' modiftype '(' ')' block
    { printf("SUB subname : modiftype ( ) block -> statement\n"); }
  | SUB subname ':' modiftype '(' subargs ')' block
    { printf("SUB subname : modiftype ( subargs ) block -> statement\n"); }
  | term ';'
    { printf("term ; -> statement\n") }
  | ';'
    { printf("; -> statement\n") }
  | if
    { printf("if -> statement\n"); }
  | FOR '(' term ';' term ';' term ')' '{' statements '}'
    { printf("FOR ( term ; term ; term ) { statements }\n"); }
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
    
words
  : WORD
  | words ',' WORD

declvar
  : MY VAR ':' modiftype
    { printf("MY VAR : modiftype -> declvar\n"); }
  | OUR VAR ':' modiftype
    { printf("OUR VAR : modiftype -> declvar\n"); }
  | HAS WORD ':' modiftype
    { printf("HAS WORD : modiftype -> declvar\n"); }

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
    {
      $$ = $1;
      printf("VAR -> term (%s)\n", ((SPerl_SVOP*)$1)->uv.pv)
    }
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
    {printf("+ term -> term\n"); }
  | '-' term %prec UMINUS
    { printf("- term -> term\n"); }
  | term ADDOP term
    {
      $$ = SPerl_newBINOP($2, 0, $1, $3);
      printf("term ADDOP(%d) term -> term\n", $2);
    }
  | term MULOP term
    { printf("term * term -> term\n"); }
  | BOOL
    { printf("BOOL -> term (%d)\n", ((SPerl_SVOP*)$1)->uv.iv); }
  | subname '(' ')'
    { printf("subname () -> term\n"); }
  | subname '(' terms  ')'
    { printf("subname (terms) -> term\n"); }
  | term RELOP term
    { printf("term RELOP term -> term %d\n", $2); }
  | term EQOP term
    { printf("term EQOP term -> term\n"); }
  | term ASSIGNOP term
    { printf("term ASSIGNOP term -> term\n"); }
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
  | SUB ':' modiftype '(' ')' block
    { printf("SUB : modiftype () block -> term\n"); }
  | SUB ':' modiftype '(' subargs ')' block
    { printf("SUB : modiftype ( subargs ) block -> term\n"); }
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
  | declvar
    { printf("declvar -> term\n"); }
  | INT
    {
      $$ = $1;
      printf("INT -> term (%d)\n", ((SPerl_SVOP*)$1)->uv.iv);
    }
  | STRING
    {
      $$ = $1;
      printf("STRING(%s) -> statement\n", ((SPerl_SVOP*)$1)->uv.pv);
    }

subname
  : WORD
    { printf("WORD -> subname\n"); }

subargs
  : subarg
    { printf("subarg -> subargs\n"); }
  | subargs ',' subarg
    { printf("subargs , subarg\n"); }

subarg
  : VAR ':' modiftype
    { printf("VAR : modiftype -> subarg (%s)\n", ((SPerl_SVOP*)$1)->uv.pv); }

modiftype
  : type
    { printf("type -> modiftype\n"); }
  | modifier type
    { printf("modifier type -> modiftype\n"); }
    
type
  : WORD
    { printf("WORD -> type (%s)\n", ((SPerl_SVOP*)$1)->uv.pv); }

modifier
  : WORD
    { printf("WORD -> modifier\n"); }
  | modifier WORD

%%

