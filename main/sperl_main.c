#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl_parser.h"
#include "../sperl_yacc.h"
#include "../sperl_array.h"
#include "../sperl_use.h"
#include "../sperl_word.h"
#include "../sperl_op.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  uint8_t* package_name = argv[1];
  
  /* initialize parser */
  SPerl_PARSER* parser = SPerl_PARSER_new();
  SPerl_ARRAY_push(parser->include_pathes, ".");
  
  /* Build use information */
  SPerl_USE* use = SPerl_USE_new(parser);
  SPerl_WORD* package_name_word = SPerl_WORD_new(parser);
  package_name_word->value = package_name;
  SPerl_OP* op_package_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD);
  op_package_name->uv.word = package_name_word;
  use->op_package_name = op_package_name;
  
  // Use OP
  SPerl_OP* op_use = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_USE);
  op_use->uv.use = use;
  
  /* Push package use infomation stack */
  SPerl_ARRAY_push(parser->op_use_stack, op_use);
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  int32_t parse_success = SPerl_yyparse(parser);
  
  SPerl_PARSER_free(parser);
  
  return parse_success;
}
