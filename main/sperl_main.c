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
  SPerl_char* pkg_name = argv[1];
  
  /* initialize parser */
  SPerl_PARSER* parser = SPerl_PARSER_new();
  SPerl_ARRAY_push(parser->include_pathes, ".");
  
  /* Build use information */
  SPerl_USE* pkg_use = SPerl_USE_new(parser);
  SPerl_WORD* pkg_name_word = SPerl_WORD_new(parser);
  pkg_name_word->value = pkg_name;
  pkg_use->pkg_name_word = pkg_name_word;
  
  /* Push pkg use infomation stack */
  SPerl_ARRAY_push(parser->use_stack, pkg_use);
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  SPerl_int parse_success = SPerl_yyparse(parser);
  
  SPerl_PARSER_free(parser);
  
  return parse_success;
}
