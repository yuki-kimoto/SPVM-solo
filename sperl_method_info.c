#include <stdlib.h>

#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"

SPerl_METHOD_INFO* SPerl_METHOD_INFO_new(SPerl_PARSER* parser) {
  SPerl_METHOD_INFO* method_info = (SPerl_METHOD_INFO*)calloc(1, sizeof(SPerl_METHOD_INFO));
  
  method_info->my_var_infos = SPerl_PARSER_new_array(parser, 0);
  method_info->my_var_info_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  return method_info;
}
