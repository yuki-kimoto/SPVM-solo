%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "sperl_base.h"
  #include "sperl_yacc.h"
  #include "sperl_toke.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
  #include "sperl_var.h"
  #include "sperl_my_var.h"
  #include "sperl_const_value.h"
  #include "sperl_word.h"
  
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
        fprintf(file, "\"%s\"", ((SPerl_WORD*)yylval.opval->uv.pv)->value);
        break;
      case VAR: {
        SPerl_VAR* var = (SPerl_VAR*)((SPerl_OP*)yylval.opval)->uv.pv;
        fprintf(file, "\"%s\"", var->name_word->value);
        break;
      }
      case CONSTVALUE: {
        SPerl_CONST_VALUE* const_value = (SPerl_CONST_VALUE*)((SPerl_OP*)yylval.opval)->uv.pv;
        
        switch(const_value->code) {
          case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
            fprintf(file, "boolean %d", const_value->uv.int_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_CHAR:
            fprintf(file, "char '%c'", (SPerl_char)const_value->uv.int_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_BYTE:
            fprintf(file, "byte %d", const_value->uv.int_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_SHORT:
            fprintf(file, "short %d", const_value->uv.int_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_INT:
            fprintf(file, "int %d", const_value->uv.int_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_LONG:
            fprintf(file, "long %ld", const_value->uv.long_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_FLOAT:
            fprintf(file, "float %f", const_value->uv.float_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_DOUBLE:
            fprintf(file, "double %f", const_value->uv.double_value);
            break;
          case SPerl_CONST_VALUE_C_CODE_STRING:
            fprintf(file, "string %s", const_value->uv.string_value);
            break;
        }
      }
    }
  }
%}

%token <opval> '+' '-'
%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <opval> LAST NEXT WORD VAR CONSTVALUE ENUM DESCRIPTER CORETYPE

%type <opval> grammar optstatements statements statement declmy declhas ifstatement elsestatement
%type <opval> block enumblock classblock declsub optdeclclassattrs callsub callop
%type <opval> optterms terms term subargs subarg optsubargs decluse declclassattr declclassattrs 
%type <opval> optdescripters listdescripters descripters enumvalues enumvalue declanonsub
%type <opval> type packagename fieldname subname package packages optenumvalues arraytype
%type <opval> forstatement whilestatement expression optpackages subtype types opttypes notsubtype
%type <opval> enumname getenumvalue getfield

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
  : optpackages
    {
      $$ = SPerl_OP_build_grammer(parser, $1);

      // Syntax error
      if (parser->error_count) {
        YYABORT;
      }
      else {
        // Dump parser infomation
        SPerl_PARSER_dump_parser(parser);
      }
    }

optpackages
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	packages
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
packages
  : packages package
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | package

package
  : PACKAGE packagename type ';'
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, $3, SPerl_OP_newOP_LIST(parser), SPerl_OP_newOP_NULL(parser));
    }
  | PACKAGE packagename classblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), SPerl_OP_newOP_LIST(parser), $3);
    }
  | PACKAGE packagename ':' listdescripters classblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
    }
  | PACKAGE packagename ':' ENUM enumblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
    }

enumblock 
  : '{' optenumvalues '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMBLOCK, $2, NULL);
    }

optenumvalues
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	enumvalues
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
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
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMVALUE, $1, NULL);
    }
  | WORD ASSIGNOP CONSTVALUE
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMVALUE, $1, $3);
    }

optstatements
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	statements
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
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
  : ifstatement
  | forstatement
  | whilestatement
  | block
  | term ';'
    {
      $$ = $1;
    }
  | expression ';'
    {
      $$ = $1;
    }
  | ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    }

forstatement
  : FOR '(' term ';' term ';' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_C_CODE_LOOP,
        $3,
        $5
      );
      SPerl_OP_sibling_splice(parser, op, $5, 0, $9);
      SPerl_OP_sibling_splice(parser, op, $9, 0, $7);
      $$ = op;
    }

whilestatement
  : WHILE '(' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_C_CODE_LOOP,
        SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL),
        $3
      );
      SPerl_OP_sibling_splice(parser, op, $3, 0, $5);
      $$ = op;
    }

ifstatement
  : IF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

elsestatement
  : /* NULL */
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

decluse
  : USE packagename ';'
    {
      $$ = SPerl_OP_build_decluse(parser, $1, $2, SPerl_OP_newOP_NULL(parser));
    }


declhas
  : HAS fieldname ':' optdescripters type ';'
    {
      $$ = SPerl_OP_build_declhas(parser, $1, $2, $4, $5);
    }

declsub
 : SUB subname '(' optsubargs ')' ':' optdescripters type block
     {
       $$ = SPerl_OP_build_declsub(parser, $1, $2, $4, $7, $8, $9);
     }

declmy
  : MY VAR ':' optdescripters type
    {
      $$ = SPerl_OP_build_declmy(parser, $1, $2, $4, $5);
    }

declanonsub
 : SUB '(' optsubargs ')' ':' optdescripters type block
     {
       $1->code = SPerl_OP_C_CODE_ANONSUB;
       $$ = SPerl_OP_build_declsub(parser, $1, SPerl_OP_newOP_NULL(parser), $3, $6, $7, $8);
     }

optdeclclassattrs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	declclassattrs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

declclassattrs
  : declclassattrs declclassattr
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | declclassattr

declclassattr
  : decluse
  | declhas
  | declsub

classblock
  : '{' optdeclclassattrs '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CLASSBLOCK, $2, NULL);
    }

expression
  : LAST
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_LAST, NULL, NULL);
    }
  | NEXT
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEXT, NULL, NULL);
    }
  | RETURN term
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_RETURN, $2, NULL);
    }

optterms
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	terms
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
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
  | CONSTVALUE
    {
      $$ = SPerl_OP_build_CONSTVALUE(parser, $1);
    }
  | declmy
  | declanonsub
  | callsub
  | callop
  | getenumvalue
  | getfield

getfield
  : VAR ARROW fieldname
    {
      $$ = SPerl_OP_build_getfield(parser, $1, $3);
    }

getenumvalue
  : enumname
    {
      $$ = SPerl_OP_build_getenumvalue(parser, $1);
    }

callop
  : '+' term %prec UMINUS
    {
      $$ = $2;
    }
  | INCOP term
    {
      $1->code = SPerl_OP_C_CODE_PREINC;
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | term INCOP
    {
      $2->code = SPerl_OP_C_CODE_POSTINC;
      SPerl_OP_sibling_splice(parser, $2, NULL, 0, $1);
      $$ = $2;
    }
  | DECOP term
    {
      $1->code = SPerl_OP_C_CODE_PREDEC;
      SPerl_OP_sibling_splice(parser, $1, NULL, 0, $2);
      $$ = $1;
    }
  | term DECOP
    {
      $2->code = SPerl_OP_C_CODE_POSTDEC;
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
      $1->code = SPerl_OP_C_CODE_NEGATE;
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
  | VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_AELEM, $1, $4);
    }

callsub
  : subname '(' optterms  ')'
    {
      $$ = SPerl_OP_build_callsub(parser, SPerl_OP_newOP_NULL(parser), $1, $3, 0);
    }
  | VAR ARROW subname '(' optterms ')'
    {
      $$ = SPerl_OP_build_callsub(parser, $1, $3, $5, 0);
    }
  | VAR ARROW '(' optterms ')'
    {
      $$ = SPerl_OP_build_callsub(parser, $1, SPerl_OP_newOP_NULL(parser), $4, 1);
    }

block 
  : '{' optstatements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_BLOCK, $2, NULL);
    }

optsubargs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	subargs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
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
  : VAR ':' optdescripters type
    {
      $$ = SPerl_OP_build_declmy(parser, SPerl_OP_newOP(parser, SPerl_OP_C_CODE_MY, NULL, NULL), $1, $3, $4);
    }
    
optdescripters
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	listdescripters

listdescripters
  :	descripters
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

descripters
  : descripters DESCRIPTER
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | DESCRIPTER

opttypes
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	types
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

types
  : types ',' type
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | type

type
  : notsubtype
  | subtype

notsubtype
  : WORD
    {
      $$ = SPerl_OP_build_wordtype(parser, $1);
    }
  | CORETYPE
  | arraytype
  
subtype
  : SUB '(' opttypes ')' type
    {
      $$ = SPerl_OP_build_subtype(parser, $3, $5);
    }

arraytype
  : notsubtype '[' ']'
    {
      $$ = SPerl_OP_build_arraytype(parser, $1);
    }
  | '(' subtype ')' '[' ']'
    {
      $$ = SPerl_OP_build_arraytype(parser, $2);
    }

enumname : WORD
fieldname : WORD
subname : WORD
packagename : WORD

%%

