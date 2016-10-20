#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl_parser.h"
#include "../sperl_array.h"
#include "../sperl_use_info.h"
#include "../sperl_word_info.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  SPerl_char* class_name = argv[1];
  
  /* initialize parser */
  SPerl_PARSER* parser = SPerl_PARSER_new();
  SPerl_ARRAY_push(parser->include_pathes, ".");
  
  /* Build use information */
  SPerl_USE_INFO* class_use_info = SPerl_USE_INFO_new(parser);
  SPerl_WORD_INFO* class_word_info = SPerl_WORD_INFO_new(parser);
  class_word_info->value = class_name;
  class_use_info->class_name = class_word_info;
  
  /* Push class use infomation stack */
  SPerl_ARRAY_push(parser->use_info_stack, class_use_info);
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  SPerl_int parse_success = SPerl_yyparse(parser);
  
  SPerl_PARSER_free(parser);
  
  return parse_success;
}
