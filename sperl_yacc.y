%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  
  #include "sperl_yacc.h"
  #include "sperl_toke.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
%}

%token <opval> '+' '-'
%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <opval> LAST NEXT WORD VAR CONSTVALUE ENUM DESCRIPTER CORETYPE

%type <opval> grammar optstatements statements statement declmy declfield ifstatement elsestatement
%type <opval> block enumblock class_block declsub opt_decl_class_attrs call_sub call_op
%type <opval> opt_terms terms term sub_args sub_arg opt_sub_args decluse decl_class_attr decl_class_attrs 
%type <opval> opt_descripters list_descripters descripters enumvalues enumvalue decl_anon_sub
%type <opval> type package_name field_name sub_name package packages optenumvalues type_array
%type <opval> forstatement whilestatement expression optpackages type_sub types opt_types not_type_sub
%type <opval> enum_name get_enum_value get_field get_array_elem convert_type

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
  : PACKAGE package_name type ';'
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, $3, SPerl_OP_newOP_LIST(parser), SPerl_OP_newOP_NULL(parser));
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name class_block
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), SPerl_OP_newOP_LIST(parser), $3);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name ':' list_descripters class_block
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name ':' ENUM enumblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
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
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POP, $1, NULL);
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
  : USE package_name ';'
    {
      $$ = SPerl_OP_build_decluse(parser, $1, $2, SPerl_OP_newOP_NULL(parser));
    }


declfield
  : HAS field_name ':' opt_descripters type ';'
    {
      $$ = SPerl_OP_build_declfield(parser, $1, $2, $4, $5);
    }

declsub
 : SUB sub_name '(' opt_sub_args ')' ':' opt_descripters type block
     {
       $$ = SPerl_OP_build_declsub(parser, $1, $2, $4, $7, $8, $9);
     }

declmy
  : MY VAR ':' opt_descripters type
    {
      $$ = SPerl_OP_build_declmy(parser, $1, $2, $4, $5);
    }

decl_anon_sub
 : SUB '(' opt_sub_args ')' ':' opt_descripters type block
     {
       $1->code = SPerl_OP_C_CODE_ANONSUB;
       $$ = SPerl_OP_build_declsub(parser, $1, SPerl_OP_newOP_NULL(parser), $3, $6, $7, $8);
     }

opt_decl_class_attrs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	decl_class_attrs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

decl_class_attrs
  : decl_class_attrs decl_class_attr
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | decl_class_attr

decl_class_attr
  : decluse
  | declfield
  | declsub

class_block
  : '{' opt_decl_class_attrs '}'
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

opt_terms
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
  | declmy
  | decl_anon_sub
  | call_sub
  | call_op
  | get_enum_value
  | get_field
  | get_array_elem
  | convert_type



convert_type
  : '(' type ')' term
    {
      $$ = SPerl_OP_build_convert_type(parser, $2, $4);
    }

get_field
  : VAR ARROW field_name
    {
      $$ = SPerl_OP_build_get_field(parser, $1, $3);
    }

get_enum_value
  : enum_name
    {
      $$ = SPerl_OP_build_get_enum_value(parser, $1);
    }

call_op
  : '+' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PLUS, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | '-' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEGATE, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | INCOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PREINC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term INCOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POSTINC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | DECOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PREDEC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term DECOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POSTDEC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | '~' term
    {
      $$ = SPerl_OP_build_call_op(parser, $1, $2, NULL);
    }
  | term '+' term %prec ADDOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ADD, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term '-' term %prec ADDOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_SUBTRACT, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term MULOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BITANDOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BITOROP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term SHIFTOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term RELOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term ASSIGNOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term ANDOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term OROP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | NOTOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $1, $2, NULL);
    }

get_array_elem
  : VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_AELEM, $1, $4);
    }

call_sub
  : sub_name '(' opt_terms  ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, SPerl_OP_newOP_NULL(parser), $1, $3, 0);
    }
  | VAR ARROW sub_name '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, $3, $5, 0);
    }
  | VAR ARROW '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, SPerl_OP_newOP_NULL(parser), $4, 1);
    }

block 
  : '{' optstatements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_BLOCK, $2, NULL);
    }

opt_sub_args
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	sub_args
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

sub_args
  : sub_args ',' sub_arg
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | sub_arg

sub_arg
  : VAR ':' opt_descripters type
    {
      $$ = SPerl_OP_build_declmy(parser, SPerl_OP_newOP(parser, SPerl_OP_C_CODE_MY, NULL, NULL), $1, $3, $4);
    }
    
opt_descripters
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	list_descripters

list_descripters
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

opt_types
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
  : not_type_sub
  | type_sub

not_type_sub
  : WORD
    {
      $$ = SPerl_OP_build_wordtype(parser, $1);
    }
  | type_array
  
type_sub
  : SUB '(' opt_types ')' type
    {
      $$ = SPerl_OP_build_type_sub(parser, $3, $5);
    }

type_array
  : not_type_sub '[' ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $1);
    }
  | '(' type_sub ')' '[' ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $2);
    }

enum_name : WORD
field_name : WORD
sub_name : WORD
package_name : WORD

%%

