#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_parser.h"
#include "sperl_op.h"
#include "sperl_class.h"
#include "sperl_collection.h"



int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  SPerl_char* file = argv[1];
  
  /* Open source file */
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    printf("Can't open file %s\n", file);
    return -1;
  }
  
  /* initialize parser */
  SPerl_yy_parser* parser = malloc(sizeof(SPerl_yy_parser));

  /* Read source file */
  size_t linestr_buf_len;
  getdelim(&(parser->linestr), &linestr_buf_len, EOF, fp);
  
  /* Close file */
  fclose(fp);
  
  /* Initialize parser information */
  parser->bufptr = parser->linestr;
  
  /* call SPerl_yyparse */
  printf("[Token reduction]\n");
  SPerl_long parse_success = SPerl_yyparse(parser);
  
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_dump_ast(parser->main_root, 0);
  
  
  free(parser->linestr);
  free(parser);
  
  return parse_success;
}
