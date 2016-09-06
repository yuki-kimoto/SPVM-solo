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

%type <opval> grammar statements block statement declvar if else terms optterms
%type <opval> term subname subargs subarg modiftype type modifier optsubargs
%type <opval> pkgname attrname

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc EQOP
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
    { printf("NULL -> grammar\n"); }
  | statements
    { printf("statements -> grammar\n"); }

statements
  : statement
    { printf("statement -> statements\n"); }
  | statements statement 
    { printf("statements statement -> statements\n"); }

statement
  : SUB subname ':' modiftype '(' optsubargs ')' block
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
  | PACKAGE pkgname ';'
    { printf("PACKAGE pkgname ; -> statement\n"); }
  | LAST ';'
    { printf("LAST ; -> statement\n"); }
  | CONTINUE ';'
    { printf("CONTINUE ; -> statement\n"); }
  | RETURN term ';'
    { printf("RETURN term ; -> statement\n"); }
  | RETURN '(' terms ')' ';'
    { printf("RETURN ( terms ) ; -> statement\n"); }
  | USE pkgname optterms';'
    { printf("USE pkgname optterms ; -> statement\n"); }

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

block 
  : '{' '}'
    { printf("{ } -> block\n"); }
  | '{' statements '}'
    { printf("{ statements } -> block\n"); }

declvar
  : MY VAR ':' modiftype
    { printf("MY VAR : modiftype -> declvar\n"); }
  | OUR VAR ':' modiftype
    { printf("OUR VAR : modiftype -> declvar\n"); }
  | HAS attrname ':' modiftype
    { printf("HAS attrname : modiftype -> declvar\n"); }

optterms
  :	/* NULL */
    { $$ = (SPerl_OP*)NULL; }
  |	terms
    { $$ = $1; }

terms
  : terms ',' term
    {
      $$ = SPerl_op_append_elem($1, $3);
      printf("terms , term -> terms\n");
    }
  | term
    {
      $$ = $1;
      printf("term -> terms\n");
    }

term
  : VAR
    {
      $$ = $1;
      printf("VAR(%s) -> term\n", ((SPerl_OP*)$1)->uv.pv)
    }
  | INT
    {
      $$ = $1;
      printf("INT(%d) -> term\n", ((SPerl_OP*)$1)->uv.iv);
    }
  | STRING
    {
      $$ = $1;
      printf("STRING(%s) -> term\n", ((SPerl_OP*)$1)->uv.pv);
    }
  | BOOL
    {
      $$ = $1;
      printf("BOOL -> term (%d)\n", ((SPerl_OP*)$1)->uv.iv);
    }
  | '+' term %prec UMINUS
    {
      $$ = $2;
      printf("+ term -> term\n");
    }
  | INCOP term
    {
      $$ = SPerl_newOP(SPerl_OP_PREINC, 0, $2, 0);
      printf("INCOP term -> term\n");
    }
  | term INCOP
    {
      $$ = SPerl_newOP(SPerl_OP_POSTINC, 0, $1, 0);
      printf("term INCOP\n");
    }
  | DECOP term
    {
      $$ = SPerl_newOP(SPerl_OP_PREDEC, 0, $2, 0);
      printf("DECOP term -> term\n");
    }
  | term DECOP
    {
      $$ = SPerl_newOP(SPerl_OP_POSTDEC, 0, $1, 0);
      printf("term DECOP -> term\n");
    }
  | NOTOP term
    {
      $$ = SPerl_newOP(SPerl_OP_NOT, 0, $2, 0);
      printf("NOTOP term -> term\n");
    }
  | '~' term
    {
      $$ = SPerl_newOP($1, 0, $2, 0);
      printf("~ term -> term\n");
    }
  | '-' term %prec UMINUS
    {
      $$ = SPerl_newOP(SPerl_OP_NEGATE, 0, $2, 0);
      printf("- term -> term\n");
    }
  | term '+' term %prec ADDOP
    {
      $$ = SPerl_newOP(SPerl_OP_ADD, 0, $1, $3);
      printf("term + term -> term\n", $2);
    }
  | term '-' term %prec ADDOP
    {
      $$ = SPerl_newOP(SPerl_OP_SUBTRACT, 0, $1, $3);
      printf("term - term -> term\n", $2);
    }
  | term MULOP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term MULOP(%d) term -> term\n", $2);
    }
  | term RELOP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term RELOP term -> term %d\n", $2);
    }
  | term EQOP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term EQOP term -> term\n");
    }
  | term BITANDOP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term BITANDOP term -> term\n");
    }
  | term BITOROP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term BITOROP term -> term\n");
    }
  | term SHIFTOP term
    {
      $$ = SPerl_newOP($2, 0, $1, $3);
      printf("term SHIFTOP term -> term\n");
    }
  | VAR ARROW '[' term ']'
    {
      $$ = SPerl_newOP(SPerl_OP_AELEM, 0, $1, $4);
      printf("VAR ARROW [ term ] -> term\n");
    }
  | VAR ARROW subname
    {
      SPerl_newOP(SPerl_OP_ATTR, 0, $1, $3);
      printf("VAR ARROW subname -> term\n");
    }
  | subname '(' optterms  ')'
    {
      $$ = SPerl_newOP(SPerl_OP_FUNC, 0, $1, $3);
      printf("subname (optterms) -> term\n");
    }
  | term ASSIGNOP term
    {
      SPerl_newOP(SPerl_OP_ASSIGN, 0, $1, $3);
      printf("term ASSIGNOP term -> term\n");
    }
  | term ANDOP term
    {
      SPerl_newOP(SPerl_OP_AND, 0, $1, $3);
      printf("term ANDOP term -> term\n");
    }
  | term OROP term
    {
      SPerl_newOP(SPerl_OP_OR, 0, $1, $3);
      printf("term OROP term -> term\n");
    }
  | VAR ARROW subname '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        0,
        $3,
        SPerl_op_append_elem($1, $5)
      );

      printf("VAR ARROW subname ( optterms )\n");
    }
  | VAR ARROW '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        0,
        $1,
        $4
      );
      
      printf("VAR ARROW ( optterms )\n");
    }
  | pkgname ARROW subname '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        0,
        SPerl_op_append_elem($1, $3),
        $5
      );
      
      printf("pkgname ARROW subname ( optterms )\n");
    }
  | declvar
    {
      $$ = (SPerl_OP*)NULL;
      printf("declvar -> term\n");
    }
  | SUB ':' modiftype '(' optsubargs ')' block
    { printf("SUB : modiftype ( optsubargs ) block -> term\n"); }

optsubargs
  :	/* NULL */
    { $$ = (SPerl_OP*)NULL; }
  |	subargs
    { $$ = $1; }

subargs
  : subarg
    { printf("subarg -> subargs\n"); }
  | subargs ',' subarg
    { printf("subargs , subarg\n"); }

subarg
  : VAR ':' modiftype
    { printf("VAR : modiftype -> subarg (%s)\n", ((SPerl_OP*)$1)->uv.pv); }

modiftype
  : type
    { printf("type -> modiftype\n"); }
  | modifier type
    { printf("modifier type -> modiftype\n"); }
    
type
  : WORD
    { printf("WORD -> type (%s)\n", ((SPerl_OP*)$1)->uv.pv); }

modifier
  : WORD
    { printf("WORD -> modifier\n"); }
  | modifier WORD

attrname
  : WORD
    {
      $$ = $1;
      printf("WORD -> attrname\n");
    }

subname
  : WORD
    {
      $$ = $1;
      printf("WORD -> subname\n");
    }

pkgname
  : WORD
    {
      $$ = $1;
      printf("WORD -> pkgname\n");
    }
%%

