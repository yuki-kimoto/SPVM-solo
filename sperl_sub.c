#include <stdlib.h>

#include "sperl_sub.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"

SPerl_SUB* SPerl_SUB_new(SPerl_PARSER* parser) {
  SPerl_SUB* sub = (SPerl_SUB*)calloc(1, sizeof(SPerl_SUB));
  
  sub->my_vars = SPerl_PARSER_new_array(parser, 0);
  sub->op_constants = SPerl_PARSER_new_array(parser, 0);
  
  return sub;
}
