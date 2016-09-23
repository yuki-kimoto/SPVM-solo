#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_parser.h"
#include "sperl_op.h"
#include "sperl_class_info.h"

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
  SPerl_yy_parser* parser = SPerl_new_parser();

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
  
  if (parse_success == 0) {
    printf("\n[Abstract Syntax Tree]\n");
    SPerl_dump_ast(parser->main_root, 0);
    
    printf("\n[Method infomation]\n");
    SPerl_dump_method_infos(parser->method_infos);
  }
  
  free(parser->linestr);
  free(parser);
  
  return parse_success;
}
