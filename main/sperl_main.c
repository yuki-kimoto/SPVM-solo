#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl_parser.h"
#include "../sperl_yacc.h"
#include "../sperl_array.h"
#include "../sperl_use.h"
#include "../sperl_word.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  SPerl_char* package_name = argv[1];
  
  /* initialize parser */
  SPerl_PARSER* parser = SPerl_PARSER_new();
  SPerl_ARRAY_push(parser->include_pathes, ".");
  
  /* Build use information */
  SPerl_USE* package_use = SPerl_USE_new(parser);
  SPerl_WORD* package_name_word = SPerl_WORD_new(parser);
  package_name_word->value = package_name;
  package_use->package_name_word = package_name_word;
  
  /* Push package use infomation stack */
  SPerl_ARRAY_push(parser->use_stack, package_use);
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  SPerl_int parse_success = SPerl_yyparse(parser);
  
  SPerl_PARSER_free(parser);
  
  return parse_success;
}
