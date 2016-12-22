%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  
  #include "sperl_yacc.h"
  #include "sperl_toke.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
  #include "sperl_dumper.h"
%}

%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE NEW
%token <opval> LAST NEXT WORD VAR CONSTANT ENUM DESCRIPTER CORETYPE UNDEF DIE
%token <opval> SWITCH CASE DEFAULT

%type <opval> grammar opt_statements statements statement decl_my decl_field if_statement else_statement
%type <opval> block enum_block class_block decl_sub opt_decl_things_in_class call_sub call_op
%type <opval> opt_terms terms term sub_args sub_arg opt_sub_args decl_use decl_thing_in_class decl_things_in_class
%type <opval> decl_enumeration_values decl_enumeration_value decl_anon_sub
%type <opval> type package_name field_name sub_name decl_package decl_things_in_grammer opt_decl_enumeration_values type_array
%type <opval> for_statement while_statement expression opt_decl_things_in_grammer type_sub types not_type_sub opt_term throw_exception
%type <opval> field array_elem convert_type decl_enum new_object array_init type_word array_length logical_op decl_thing_in_grammer
%type <opval> switch_statement case_statement default_statement
%type <opval> ';'

%right <opval> ASSIGN
%left <opval> OR
%left <opval> AND
%left <opval> BIT_OR BIT_XOR
%left <opval> BIT_AND
%nonassoc <opval> REL
%left <opval> SHIFT
%left <opval> '+' '-'
%left <opval> MULTIPLY DIVIDE REMAINDER
%right <opval> NOT '~' '@' UMINUS
%nonassoc <opval> INC DEC
%left <opval> ARROW
%nonassoc <opval> ')'
%left <opval> '('
%left <opval> '[' '{'

%%

grammar
  : opt_decl_things_in_grammer
    {
      $$ = SPerl_OP_build_grammer(parser, $1);

      // Syntax error
      if (parser->error_count) {
        YYABORT;
      }
      else {
        // Dump parser infomation
        SPerl_DUMPER_dump_parser(parser);
      }
    }

opt_decl_things_in_grammer
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	decl_things_in_grammer
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
  
decl_things_in_grammer
  : decl_things_in_grammer decl_thing_in_grammer
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | decl_thing_in_grammer

decl_thing_in_grammer
  : decl_use
  | decl_package

decl_package
  : PACKAGE package_name class_block
    {
      $$ = SPerl_OP_build_decl_package(parser, $1, $2, $3);
      if (parser->fatal_error) {
        YYABORT;
      }
    }

enum_block 
  : '{' opt_decl_enumeration_values '}'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_ENUM_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $2);
    }

opt_decl_enumeration_values
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	decl_enumeration_values
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
decl_enumeration_values
  : decl_enumeration_values ',' decl_enumeration_value 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | decl_enumeration_value
  
decl_enumeration_value
  : WORD
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $1);
    }
  | WORD ASSIGN CONSTANT
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $$, $1, 0, $3);
    }

throw_exception
  : DIE term

opt_statements
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	statements
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
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
  : if_statement
  | for_statement
  | while_statement
  | block
  | term ';'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_POP, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $1);
    }
  | expression ';'
    {
      $$ = $1;
    }
  | ';'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line);
    }
  | switch_statement
  | case_statement
  | default_statement

for_statement
  : FOR '(' opt_term ';' term ';' opt_term ')' block
    {
      $$ = SPerl_OP_build_for_statement(parser, $1, $3, $5, $7, $9);
    }

while_statement
  : WHILE '(' term ')' block
    {
      $$ = SPerl_OP_build_while_statement(parser, $1, $3, $5);
    }

switch_statement
  : SWITCH '(' term ')' block
    {
      $$ = SPerl_OP_build_switch_statement(parser, $1, $3, $5)
    }

case_statement
  : CASE term ':'
    {
      $$ = SPerl_OP_build_case_statement(parser, $1, $2)
    }

default_statement
  : DEFAULT ':'

if_statement
  : IF '(' term ')' block else_statement
    {
      $$ = SPerl_OP_build_if_statement(parser, $1, $3, $5, $6);
    }

else_statement
  : /* NULL */
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, parser->cur_module_path, parser->cur_line);
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block else_statement
    {
      $$ = SPerl_OP_build_if_statement(parser, $1, $3, $5, $6);
    }

decl_use
  : USE package_name ';'
    {
      $$ = SPerl_OP_build_decl_use(parser, $1, $2);
    }


decl_field
  : HAS field_name ':' type ';'
    {
      $$ = SPerl_OP_build_decl_field(parser, $1, $2, $4);
    }

decl_sub
 : SUB sub_name '(' opt_sub_args ')' ':' type block
     {
       $$ = SPerl_OP_build_decl_sub(parser, $1, $2, $4, $7, $8);
     }

decl_enum
  : ENUM enum_block
    {
      $$ = SPerl_OP_build_decl_enum(parser, $1, $2);
    }

decl_my
  : MY VAR ':' type
    {
      $$ = SPerl_OP_build_decl_my(parser, $1, $2, $4);
    }
  | MY VAR
    {
      SPerl_OP* op_type = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $2->file, $2->line);
      
      $$ = SPerl_OP_build_decl_my(parser, $1, $2, op_type);
    }

decl_anon_sub
 : SUB '(' ')' ':' type block
     {
       $1->code = SPerl_OP_C_CODE_DECL_ANON_SUB;
       SPerl_OP* op_sub_args = SPerl_OP_newOP_LIST_(parser, $2->file, $2->line);
       $$ = SPerl_OP_build_decl_sub(parser, $1, SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line), op_sub_args, $5, $6);
     }
 | SUB '(' sub_args ')' ':' type block
     {
       $1->code = SPerl_OP_C_CODE_DECL_ANON_SUB;
       $$ = SPerl_OP_build_decl_sub(parser, $1, SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line), $3, $6, $7);
     }

opt_decl_things_in_class
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	decl_things_in_class
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

decl_things_in_class
  : decl_things_in_class decl_thing_in_class
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | decl_thing_in_class

decl_thing_in_class
  : decl_field
  | decl_sub
  | decl_enum

class_block
  : '{' opt_decl_things_in_class '}'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_CLASS_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $2);
    }

expression
  : LAST
  | NEXT
  | RETURN term
    {
      $$ = $1;
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $2);
    }
  | throw_exception

opt_terms
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	terms
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
terms
  : terms ',' term
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | term

array_length
  : '@' term
    {
      $$ = SPerl_OP_build_array_length(parser, $1, $2);
    }

opt_term
  : /* NULL */
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, parser->cur_module_path, parser->cur_line);
    }
  | term
term
  : VAR
  | CONSTANT
  | decl_my
  | decl_anon_sub
  | call_sub
  | call_op
  | field
  | array_elem
  | convert_type
  | UNDEF
  | new_object
  | array_init
  | array_length
  | logical_op

array_init
  : '[' opt_terms ']'
    {
      $$ = SPerl_OP_build_array_init(parser, $2);
    }

new_object
  : NEW type
    {
      $$ = SPerl_OP_build_new_object(parser, $1, $2);
    }

convert_type
  : '(' type ')' term
    {
      $$ = SPerl_OP_build_convert_type(parser, $2, $4);
    }

field
  : VAR ARROW field_name
    {
      $$ = SPerl_OP_build_field(parser, $1, $3);
    }

call_op
  : '+' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_PLUS, $1->file, $1->line);
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | '-' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NEGATE, $1->file, $1->line);
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | INC term
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_PRE_INC, $1->file, $1->line);
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term INC
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_POST_INC, $2->file, $2->line);
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | DEC term
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_PRE_DEC, $1->file, $1->line);
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term DEC
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_POST_DEC, $2->file, $2->line);
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | '~' term
    {
      $$ = SPerl_OP_build_call_op(parser, $1, $2, NULL);
    }
  | term '+' term
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_ADD, $2->file, $2->line);
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term '-' term
    {
      SPerl_OP* op = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_SUBTRACT, $2->file, $2->line);
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term MULTIPLY term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term DIVIDE term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term REMAINDER term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BIT_XOR term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BIT_AND term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BIT_OR term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term SHIFT term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term REL term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term ASSIGN term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | '(' term ')'
    {
      $$ = $2;
    }

logical_op
  : term AND term
    {
      $$ = SPerl_OP_build_logical_op(parser, $2, $1, $3);
    }
  | term OR term
    {
      $$ = SPerl_OP_build_logical_op(parser, $2, $1, $3);
    }
  | NOT term
    {
      $$ = SPerl_OP_build_logical_op(parser, $1, $2, NULL);
    }

array_elem
  : VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_ARRAY_ELEM, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $1);
      SPerl_OP_sibling_splice(parser, $$, $1, 0, $4);
      
      $$->file = $1->file;
      $$->line = $1->line;
    }

call_sub
  : sub_name '(' opt_terms  ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line), $1, $3, 0);
    }
  | VAR ARROW sub_name '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, $3, $5, 0);
    }
  | VAR ARROW '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line), $4, 1);
    }

block 
  : '{' opt_statements '}'
    {
      $$ = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_BLOCK, $1->file, $1->line);
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $2);
    }

opt_sub_args
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
    }
  |	sub_args
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST_(parser, $1->file, $1->line);
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
  : VAR ':' type
    {
      $$ = SPerl_OP_build_decl_my(parser, SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_DECL_MY_VAR, $1->file, $1->line), $1, $3);
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
  : type_word
  | type_array

type_word
  : WORD
    {
      $$ = SPerl_OP_build_type_word(parser, $1);
    }

type_sub
  : SUB '(' ')' type
    {
      SPerl_OP* op_types = SPerl_OP_newOP_LIST_(parser, parser->cur_module_path, parser->cur_line);
      $$ = SPerl_OP_build_type_sub(parser, op_types, $4);
    }
  | SUB '(' types ')' type
    {
      $$ = SPerl_OP_build_type_sub(parser, $3, $5);
    }

type_array
  : not_type_sub '[' ']'
    {
      SPerl_OP* op_null = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line);
      $$ = SPerl_OP_build_type_array(parser, $1, op_null);
    }
  | not_type_sub '[' term ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $1, $3);
    }
  | '(' type_sub ')' '[' ']'
    {
      SPerl_OP* op_null = SPerl_OP_newOP_(parser, SPerl_OP_C_CODE_NULL, $1->file, $1->line);
      $$ = SPerl_OP_build_type_array(parser, $2, op_null);
    }
  | '(' type_sub ')' '[' term ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $2, $5);
    }

field_name : WORD
sub_name : WORD
package_name : WORD

%%

