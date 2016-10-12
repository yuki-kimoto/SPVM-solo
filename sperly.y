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
  #include "sperl_const_info.h"
  
  // Print token value for debug
  static void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval) {
    
    switch(type) {
      case MULOP:
        fprintf(file, "%d", yylval.ival);
        break;
      case WORD:
        fprintf(file, "\"%s\"", ((SPerl_OP*)yylval.opval)->uv.pv);
        break;
      case VAR: {
        SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)((SPerl_OP*)yylval.opval)->uv.pv;
        fprintf(file, "\"%s\"", var_info->name);
        break;
      }
      case CONST: {
        SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)((SPerl_OP*)yylval.opval)->uv.pv;
        
        switch(const_info->type) {
          case SPerl_CONST_INFO_BOOLEAN:
            fprintf(file, "boolean %d", const_info->uv.int_value);
            break;
          case SPerl_CONST_INFO_CHAR:
            fprintf(file, "char '%c'", (SPerl_char)const_info->uv.int_value);
            break;
          case SPerl_CONST_INFO_BYTE:
            fprintf(file, "byte %d", const_info->uv.int_value);
            break;
          case SPerl_CONST_INFO_SHORT:
            fprintf(file, "short %d", const_info->uv.int_value);
            break;
          case SPerl_CONST_INFO_INT:
            fprintf(file, "int %d", const_info->uv.int_value);
            break;
          case SPerl_CONST_INFO_LONG:
            fprintf(file, "long %ld", const_info->uv.long_value);
            break;
          case SPerl_CONST_INFO_FLOAT:
            fprintf(file, "float %f", const_info->uv.float_value);
            break;
          case SPerl_CONST_INFO_DOUBLE:
            fprintf(file, "double %f", const_info->uv.double_value);
            break;
          case SPerl_CONST_INFO_STRING:
            fprintf(file, "string %s", const_info->uv.string_value);
            break;
        }
      }
    }
  }
%}

%token <opval> '+' '-'
%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <opval> LAST NEXT WORD VAR CONST

%type <opval> grammar statements statement declmy declhas if else block
%type <opval> optterms terms term subargs subarg optsubargs
%type <opval> desctype descripters descripter
%type <opval> type pkgname fieldname subname package packages pkgalias

%right <opval> ASSIGNOP
%left <opval> OROP
%left <opval> ANDOP
%left <opval> BITOROP
%left <opval> BITANDOP
%nonassoc <opval> RELOP
%left <opval> SHIFTOP
%left <opval> ADDOP
%left <opval> MULOP
%right <opval> NOTOP '~' UMINUS
%nonassoc <opval> INCOP DECOP
%left <opval> ARROW
%nonassoc <opval> ')'
%left <opval> '('
%left <opval> '[' '{'

%%

grammar
  : /* NULL */
    {
      $$ = NULL;
    }
  | packages
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_GRAMMER, $1, NULL);
      if (parser->error_count) {
        YYABORT;
      }
      
      // Build constant pool
      SPerl_OP_build_const_pool(parser);
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
      $$ = SPerl_OP_build_PACKAGE(parser, $1, $2, $3, NULL);
    }
  | PACKAGE pkgname ':' descripters block
    {
      $$ = SPerl_OP_build_PACKAGE(parser, $1, $2, $5, $4);
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
      $$ = SPerl_OP_build_SUB(parser, $1, $2, $4, $7, $8);
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
      $$ = SPerl_OP_build_USE(parser, $1, $2, NULL);
    }
  | USE pkgname '-' pkgalias';'
    {
      $$ = SPerl_OP_build_USE(parser, $1, $2, $4);
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
      $$ = SPerl_OP_build_MY(parser, $1, $2, $4);
    }

declhas
  : HAS fieldname ':' desctype
    {
      $$ = SPerl_OP_build_HAS(parser, $1, $2, $4);
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
      $$ = SPerl_OP_build_CONST(parser, $1);
    }
  | '+' term %prec UMINUS
    {
      $$ = $2;
    }
  | INCOP term
    {
      $1->type = SPerl_OP_PREINC;
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | term INCOP
    {
      $2->type = SPerl_OP_POSTINC;
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      $$ = $2;
    }
  | DECOP term
    {
      $1->type = SPerl_OP_PREDEC;
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | term DECOP
    {
      $2->type = SPerl_OP_POSTDEC;
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      $$ = $2;
    }
  | NOTOP term
    {
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | '~' term
    {
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | '-' term %prec UMINUS
    {
      $1->type = SPerl_OP_NEGATE;
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | term '+' term %prec ADDOP
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term '-' term %prec ADDOP
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term MULOP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term RELOP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term BITANDOP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term BITOROP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term SHIFTOP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
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
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term ANDOP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
    }
  | term OROP term
    {
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $2, $1, 0, $3);
      $$ = $2;
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
      $$ = SPerl_OP_build_MY(parser, NULL, $1, $3);
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
pkgalias : WORD

%%

