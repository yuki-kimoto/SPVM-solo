#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../spvm_.h"
#include "../spvm_parser.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  // Package name
  const char* package_name = argv[1];
  
  // Create spvm
  SPVM_* spvm = SPVM_new();
  
  spvm->parser->entry_point_package_name = package_name;
  
  // Run
  SPVM_run(spvm, package_name);
  
  // Free spvm
  SPVM_free(spvm);
  
  return 0;
}
