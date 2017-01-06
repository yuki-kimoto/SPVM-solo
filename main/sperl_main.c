#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl.h"
#include "../sperl_parser.h"
#include "../sperl_array.h"
#include "../sperl_vm.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  // Package name
  const char* package_name = argv[1];
  
  // Parse
  SPerl* sperl = SPerl_new();
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_ARRAY_push(parser->include_pathes, ".");
  int32_t parse_success = SPerl_PARSER_parse(sperl, package_name);
  
  // Entry point
  const char* entry_point = parser->entry_point;
  
  // Run
  SPerl_VM_run(sperl, entry_point);
  
  // Free sperl
  SPerl_free(sperl);
  
  return parse_success;
}
