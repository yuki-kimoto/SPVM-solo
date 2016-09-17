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
%token <ival> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <ival> RELOP
%token <ival> LAST NEXT AS
%token <opval> WORD VAR INT DOUBLE STRING CHAR BOOL 

%type <opval> grammar statements statement declvar if else block
%type <opval> optterms terms term subargs subarg optsubargs
%type <opval> modiftype modifiers modifier
%type <opval> type pkgname attrname subname

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
      $$ = SPerl_newOP(SPerl_OP_GRAMMER, NULL, NULL);;
      printf("NULL -> grammar\n");
    }
  | statements
    {
      SPerl_OP* op = SPerl_newOP(SPerl_OP_GRAMMER, $1, NULL);
      parser->main_root = op;
      $$ = op;
      printf("statements -> grammar\n");
    }

statements
  : statements statement 
    {
      $$ = SPerl_op_append_elem(
        $1,
        SPerl_newOP(SPerl_OP_STATEMENT, $2, NULL)
      );
      printf("statements statement -> statements\n");
    }
  | statement
    {
      $$ = SPerl_newOP(SPerl_OP_STATEMENT, $1, NULL);
      printf("statement -> statements\n");
    }

statement
  : SUB subname ':' modiftype '(' optsubargs ')' block
    {
      SPerl_OP* optsubargs_op = $6;
      if (!optsubargs_op) {
        optsubargs_op = SPerl_newOP(SPerl_OP_NULL, NULL, NULL);
      }
      SPerl_OP* op = SPerl_newOP(SPerl_OP_SUB, $2, $4);
      SPerl_op_sibling_splice(op, $4, 0, optsubargs_op);
      SPerl_op_sibling_splice(op, optsubargs_op, 0, $8);
      
      $$ = op;
      
      printf("SUB subname : modiftype ( optsubargs ) block -> statement\n");
    }
  | FOR '(' term ';' term ';' term ')' block
    {
      SPerl_OP* op = SPerl_newOP(
        SPerl_OP_LOOP,
        $3,
        $5
      );
      SPerl_op_sibling_splice(op, $5, 0, $9);
      SPerl_op_sibling_splice(op, $9, 0, $7);
      $$ = op;
      printf("FOR ( term ; term ; term ) block\n");
    }
  | WHILE '(' term ')' block
    {
      SPerl_OP* op = SPerl_newOP(
        SPerl_OP_LOOP,
        SPerl_newOP(SPerl_OP_NULL, NULL, NULL),
        $3
      );
      SPerl_op_sibling_splice(op, $3, 0, $5);
      $$ = op;
      
      printf("WHILE ( term ) block\n");
    }
  | term ';'
    {
      $$ = $1;
      printf("term ; -> statement\n")
    }
  | ';'
    {
      $$ = (SPerl_OP*)NULL;
      printf("; -> statement\n")
    }
  | if
    {
      $$ = $1;
      printf("if -> statement\n");
    }
  | PACKAGE pkgname ';'
    {
      $$ = SPerl_newOP(SPerl_OP_PACKAGE, $2, NULL);
      printf("PACKAGE pkgname ; -> statement\n");
    }
  | LAST ';'
    {
      $$ = SPerl_newOP(SPerl_OP_LAST, NULL, NULL);
      printf("LAST ; -> statement\n"); }
  | NEXT ';'
    {
      $$ = SPerl_newOP(SPerl_OP_NEXT, NULL, NULL);
      printf("NEXT ; -> statement\n");
    }
  | RETURN term ';'
    {
      $$ = SPerl_newOP(SPerl_OP_RETURN, $2, NULL);
      printf("RETURN term ; -> statement\n");
    }
  | USE pkgname';'
    {
      $$ = SPerl_newOP(SPerl_OP_USE, $2, NULL);
      printf("USE pkgname; -> statement\n");
    }
  | USE pkgname AS WORD';'
    {
      $$ = SPerl_newOP(SPerl_OP_USE, $2, $4);
      printf("USE pkgname AS WORD; -> statement\n");
    }
  | block
    {
      $$ = $1;
      printf("block -> statement\n");
    }

if
  : IF '(' term ')' block else
    {
      SPerl_OP* op = SPerl_newOP(SPerl_OP_COND, $3, $5);
      if ($6) {
        SPerl_op_sibling_splice(op, $5, 0, $6);
      }
      $$ = op;
      printf("IF ( term ) block -> if\n");
    }

else
  : /* NULL */
    {
      $$ = (SPerl_OP*)0;
      printf("NULL -> else\n")
    };
  | ELSE block
    {
      $$ = $2;
      printf("ELSE block -> else\n");
    }
  | ELSIF '(' term ')' block else
    {
      SPerl_OP* op = SPerl_newOP(SPerl_OP_COND, $3, $5);
      if ($6) {
        SPerl_op_sibling_splice(op, $5, 0, $6);
      }
      $$ = op;
      
      printf("ELSIF ( term ) block else -> else\n");
    }

declvar
  : MY VAR ':' modiftype
    {
      $$ = SPerl_newOP(SPerl_OP_MY, $2, $4);
      printf("MY VAR : modiftype -> declvar\n");
    }
  | HAS attrname ':' modiftype
    {
      $$ = SPerl_newOP(SPerl_OP_HAS, $2, $4);
      printf("HAS attrname : modiftype -> declvar\n");
    }

optterms
  :	/* NULL */
    {
      $$ = (SPerl_OP*)NULL;
      printf("NULL -> optterms\n");
    }
  |	terms
    {
      $$ = $1;
      printf("terms -> optterms\n");
    }

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
  | DOUBLE
    {
      $$ = $1;
      printf("DOUBLE(%f) -> term\n", ((SPerl_OP*)$1)->uv.nv);
    }
  | STRING
    {
      $$ = $1;
      printf("STRING(%s) -> term\n", ((SPerl_OP*)$1)->uv.pv);
    }
  | CHAR
    {
      $$ = $1;
      printf("CHAR(%c) -> term\n", ((SPerl_OP*)$1)->uv.char_value);
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
      $$ = SPerl_newOP(SPerl_OP_PREINC, $2, NULL);
      printf("INCOP term -> term\n");
    }
  | term INCOP
    {
      $$ = SPerl_newOP(SPerl_OP_POSTINC, $1, NULL);
      printf("term INCOP\n");
    }
  | DECOP term
    {
      $$ = SPerl_newOP(SPerl_OP_PREDEC, $2, NULL);
      printf("DECOP term -> term\n");
    }
  | term DECOP
    {
      $$ = SPerl_newOP(SPerl_OP_POSTDEC, $1, NULL);
      printf("term DECOP -> term\n");
    }
  | NOTOP term
    {
      $$ = SPerl_newOP(SPerl_OP_NOT, $2, NULL);
      printf("NOTOP term -> term\n");
    }
  | '~' term
    {
      $$ = SPerl_newOP($1, $2, NULL);
      printf("~ term -> term\n");
    }
  | '-' term %prec UMINUS
    {
      $$ = SPerl_newOP(SPerl_OP_NEGATE, $2, NULL);
      printf("- term -> term\n");
    }
  | term '+' term %prec ADDOP
    {
      $$ = SPerl_newOP(SPerl_OP_ADD, $1, $3);
      printf("term + term -> term\n", $2);
    }
  | term '-' term %prec ADDOP
    {
      $$ = SPerl_newOP(SPerl_OP_SUBTRACT, $1, $3);
      printf("term - term -> term\n", $2);
    }
  | term MULOP term
    {
      $$ = SPerl_newOP($2, $1, $3);
      printf("term MULOP(%d) term -> term\n", $2);
    }
  | term RELOP term
    {
      $$ = SPerl_newOP($2, $1, $3);
      printf("term RELOP term -> term %d\n", $2);
    }
  | term BITANDOP term
    {
      $$ = SPerl_newOP($2, $1, $3);
      printf("term BITANDOP term -> term\n");
    }
  | term BITOROP term
    {
      $$ = SPerl_newOP($2, $1, $3);
      printf("term BITOROP term -> term\n");
    }
  | term SHIFTOP term
    {
      $$ = SPerl_newOP($2, $1, $3);
      printf("term SHIFTOP term -> term\n");
    }
  | VAR ARROW '[' term ']'
    {
      $$ = SPerl_newOP(SPerl_OP_AELEM, $1, $4);
      printf("VAR ARROW [ term ] -> term\n");
    }
  | VAR ARROW subname
    {
      $$ = SPerl_newOP(SPerl_OP_ATTR, $1, $3);
      printf("VAR ARROW subname -> term\n");
    }
  | subname '(' optterms  ')'
    {
      $$ = SPerl_newOP(SPerl_OP_FUNC, $1, $3);
      printf("subname (optterms) -> term\n");
    }
  | term ASSIGNOP term
    {
      $$ = SPerl_newOP(SPerl_OP_ASSIGN, $1, $3);
      printf("term ASSIGNOP term -> term\n");
    }
  | term ANDOP term
    {
      $$ = SPerl_newOP(SPerl_OP_AND, $1, $3);
      printf("term ANDOP term -> term\n");
    }
  | term OROP term
    {
      $$ = SPerl_newOP(SPerl_OP_OR, $1, $3);
      printf("term OROP term -> term\n");
    }
  | VAR ARROW subname '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        $3,
        SPerl_op_append_elem($1, $5)
      );

      printf("VAR ARROW subname ( optterms )\n");
    }
  | VAR ARROW '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        $1,
        $4
      );
      
      printf("VAR ARROW ( optterms )\n");
    }
  | pkgname ARROW subname '(' optterms ')'
    {
      $$ = SPerl_newOP(
        SPerl_OP_FUNC,
        SPerl_op_append_elem($1, $3),
        $5
      );
      
      printf("pkgname ARROW subname ( optterms )\n");
    }
  | declvar
    {
      $$ = $1;
      printf("declvar -> term\n");
    }
  | SUB ':' modiftype '(' optsubargs ')' block
    {
      SPerl_OP* op_optsubargs = $5;
      if (!op_optsubargs) {
        op_optsubargs = SPerl_newOP(SPerl_OP_NULL, NULL, NULL);
      }
      SPerl_OP* op = SPerl_newOP(SPerl_OP_ANONSUB, $3, op_optsubargs);
      SPerl_op_sibling_splice(op, op_optsubargs, 0, $7);
      $$ = op;
      
      printf("SUB : modiftype ( optsubargs ) block -> term\n");
    }

block 
  : '{' '}'
    {
      $$ = SPerl_newOP(SPerl_OP_BLOCK, NULL, NULL);
      printf("{ } -> block\n");
    }
  | '{' statements '}'
    {
      $$ = SPerl_newOP(SPerl_OP_BLOCK, $2, NULL);
      printf("{ statements } -> block\n");
    }

optsubargs
  :	/* NULL */
    {
      $$ = (SPerl_OP*)NULL;
      printf("NULL -> optsubargs\n");
    }
  |	subargs
    {
      $$ = $1;
      printf("subargs -> optsubargs\n");
    }

subargs
  : subargs ',' subarg
    {
      $$ = SPerl_op_append_elem($1, $3);
      printf("subargs , subarg\n");
    }
  | subarg
    {
      $$ = $1;
      printf("subarg -> subargs\n");
    }

subarg
  : VAR ':' modiftype
    {
      $$ = SPerl_newOP(SPerl_OP_SUBARG, $1, $3);
      printf("VAR : modiftype -> subarg (%s)\n", ((SPerl_OP*)$1)->uv.pv);
    }

modiftype
  : type
    {
      $$ = $1;
      printf("type -> modiftype\n");
    }
  | modifiers type
    {
      $$ = SPerl_op_append_elem($2, $1);
      printf("modifiers type -> modiftype\n");
    }
    
type
  : WORD
    {
      $$ = $1;
      printf("WORD -> type (%s)\n", ((SPerl_OP*)$1)->uv.pv);
    }

modifiers
  : modifiers  modifier
    {
      $$ = SPerl_op_append_elem($1, $2);
      printf("modifiers modifier -> modifiers\n");
    }
  | modifier
    {
      $$ = $1;
      printf("modifier -> modifiers\n");
    }

modifier
  : WORD
    {
      $$ = $1;
      printf("WORD -> modifier\n");
    }

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

