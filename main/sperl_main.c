#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl.h"
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
  SPerl_ARRAY_push(sperl->include_pathes, ".");
  int32_t parse_success = SPerl_parse(sperl, package_name);
  
  // Run
  SPerl_VM_run(sperl);
  
  // Free sperl
  SPerl_free(sperl);
  
  return parse_success;
}
