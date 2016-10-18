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
  #include "sperl_word_info.h"

  /* Function for error */
  void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s)
  {
    parser->error_count++;
    
    if (memcmp(s, "Error:", 6) == 0) {
      fprintf(stderr, "%s", s);
    }
    // Syntax structure error
    else {
      // Current token
      SPerl_int length = 0;
      SPerl_int empty_count = 0;
      SPerl_char* ptr = parser->befbufptr;
      while (ptr != parser->bufptr) {
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
          empty_count++;
        }
        else {
          length++;
        }
        ptr++;
      }
      SPerl_char* token = calloc(length + 1, sizeof(SPerl_char));
      memcpy(token, parser->befbufptr + empty_count, length);
      token[length] = '\0';
      
      fprintf(stderr, "Error: unexpected token \"%s\" at %s line %d\n", token, parser->cur_file, parser->cur_line);
    }
  }

  // Print token value for debug
  void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval) {
    
    switch(type) {
      case WORD:
        fprintf(file, "\"%s\"", ((SPerl_WORD_INFO*)yylval.opval->uv.pv)->value);
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
%token <opval> LAST NEXT WORD VAR CONST ENUM

%type <opval> grammar optstatements statements statement declmy declhas ifstatement elsestatement block enumblock classblock declsub
%type <opval> optterms terms term subargs subarg optsubargs decluse declusehassub declusehassubs optdeclusehassubs
%type <opval> desctype descripters descripter enumvalues enumvalue declanonsub
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
      $$ = SPerl_OP_build_GRAMMER(parser, NULL);
    }
  | packages
    {
      $$ = SPerl_OP_build_GRAMMER(parser, $1);

      // Syntax error
      if (parser->error_count) {
        YYABORT;
      }
      else {
        // Dump parser infomation
        SPerl_PARSER_dump_parser_info(parser);
      }
    }

packages
  : packages package
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | package

package
  : PACKAGE pkgname classblock
    {
      $$ = SPerl_OP_build_PACKAGE(parser, $1, $2, $3, NULL);
    }
  | PACKAGE pkgname ':' descripters classblock
    {
      $$ = SPerl_OP_build_PACKAGE(parser, $1, $2, $5, $4);
    }
  | PACKAGE pkgname ':' ENUM enumblock
    {
      $$ = SPerl_OP_build_PACKAGE(parser, $1, $2, $5, $4);
    }

enumblock 
  : '{' '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ENUMBLOCK, NULL, NULL);
    }
  | '{' enumvalues '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ENUMBLOCK, $2, NULL);
    }

enumvalues
  : enumvalues ',' enumvalue 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | enumvalue
  
enumvalue
  : WORD
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ENUMVALUE, $1, NULL);
    }
  | WORD ASSIGNOP CONST
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_ENUMVALUE, $1, $3);
    }

optstatements
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	statements
    {
      if ($1->type == SPerl_OP_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
statements
  : statements statement 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | statement

statement
  : FOR '(' term ';' term ';' term ')' block
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
    {
      $$ = $1;
    }
  | ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
    }
  | ifstatement
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
  | block

ifstatement
  : IF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

elsestatement
  : /* NULL */
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_NULL, NULL, NULL);
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

decluse
  : USE pkgname ';'
    {
      $$ = SPerl_OP_build_USE(parser, $1, $2, SPerl_OP_newOP_NULL(parser));
    }
  | USE pkgname '-' pkgalias';'
    {
      $$ = SPerl_OP_build_USE(parser, $1, $2, $4);
    }

declhas
  : HAS fieldname ':' desctype ';'
    {
      $$ = SPerl_OP_build_HAS(parser, $1, $2, $4);
    }

declsub
 : SUB subname '(' optsubargs ')' ':' desctype block
     {
       $$ = SPerl_OP_build_SUB(parser, $1, $2, $4, $7, $8);
     }

declmy
  : MY VAR ':' desctype
    {
      $$ = SPerl_OP_build_MY(parser, $1, $2, $4);
    }

declanonsub
 : SUB '(' optsubargs ')' ':' desctype block
     {
       $$ = SPerl_OP_build_SUB(parser, $1, NULL, $3, $6, $7);
     }

optdeclusehassubs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	declusehassubs
    {
      if ($1->type == SPerl_OP_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

declusehassubs
  : declusehassubs declusehassub
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | declusehassub

declusehassub
  : decluse
  | declhas
  | declsub

classblock
  : '{' optdeclusehassubs '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_CLASSBLOCK, $2, NULL);
    }

optterms
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	terms
    {
      if ($1->type == SPerl_OP_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
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
  | declanonsub

block 
  : '{' optstatements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_BLOCK, $2, NULL);
    }

optsubargs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	subargs
    {
      if ($1->type == SPerl_OP_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

subargs
  : subargs ',' subarg
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | subarg

subarg
  : VAR ':' desctype
    {
      $$ = SPerl_OP_build_MY(parser, SPerl_OP_newOP(parser, SPerl_OP_MY, NULL, NULL), $1, $3);
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

