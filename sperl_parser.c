#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_parser.h"

SPerl_yy_parser* SPerl_new_parser() {
  SPerl_yy_parser* parser = (SPerl_yy_parser*)malloc(sizeof(SPerl_yy_parser));
  memset(parser, 0, sizeof(SPerl_yy_parser));
  
  parser->method_infos = SPerl_ARRAY_new(0);
  
  return parser;
}
