#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl_parser.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  // Package name
  uint8_t* package_name = argv[1];
  
  // Parse
  SPerl_PARSER* parser = SPerl_PARSER_new();
  SPerl_ARRAY_push(parser->include_pathes, ".");
  int32_t parse_success = SPerl_PARSER_parse(parser, package_name);
  
  // Run
  SPerl_VM_run(parser);
  
  // Free parser
  SPerl_PARSER_free(parser);
  
  return parse_success;
}
