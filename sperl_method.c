#include <stdlib.h>

#include "sperl_method.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"

SPerl_METHOD* SPerl_METHOD_new(SPerl_PARSER* parser) {
  SPerl_METHOD* method = (SPerl_METHOD*)calloc(1, sizeof(SPerl_METHOD));
  
  method->my_vars = SPerl_PARSER_new_array(parser, 0);
  
  return method;
}
