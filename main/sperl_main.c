#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sperl.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  // Package name
  const char* package_name = argv[1];
  
  // Create sperl
  SPerl* sperl = SPerl_new();
  
  // Run
  SPerl_run(sperl, package_name);
  
  // Free sperl
  SPerl_free(sperl);
  
  return 0;
}
