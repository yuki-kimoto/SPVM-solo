#include <stdio.h>
#include <stdlib.h>

#include "static_parser.h"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  char* file = argv[1];
  
  /* Open source file */
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    printf("Can't open file %s\n", file);
    return -1;
  }
  
  /* initialize parser */
  static_yy_parser* parser = malloc(sizeof(static_yy_parser));

  /* Read source file */
  size_t linestr_buf_len;
  getdelim(&(parser->linestr), &linestr_buf_len, EOF, fp);
  
  /* Close file */
  fclose(fp);
  
  /* Initialize parser information */
  parser->bufptr = parser->linestr;
  
  /* call static_yyparse */
  int parse_success = static_yyparse(parser);
  
  free(parser->linestr);
  free(parser);
  
  return parse_success;
}
