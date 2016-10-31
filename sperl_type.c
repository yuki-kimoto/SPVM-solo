#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_type_word.h"
#include "sperl_type_array.h"
#include "sperl_type_sub.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"
#include "sperl_word.h"

SPerl_char* const SPerl_TYPE_C_CODE_NAMES[] = {
  "word",
  "array",
  "sub",
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_TYPE));
}

SPerl_char* SPerl_TYPE_to_string(SPerl_PARSER* parser, SPerl_TYPE* type) {
  
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    return type->uv.type_word->name_word->value;
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_int depth = 0;
    while (1) {
      if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
        depth++;
        type = type->uv.type_array->type;
      }
      else {
        SPerl_char* str_first = SPerl_TYPE_to_string(parser, type);
        SPerl_int str_first_len = strlen(str_first);
        SPerl_int str_len = str_first_len + 2 * depth;
        SPerl_char* str = SPerl_PARSER_new_string(parser, str_len);
        memcpy(str, str_first, strlen(str_first));
        for (SPerl_int j = 0; j < depth; j++) {
          str[str_first_len + (j * 2)] = '[';
          str[str_first_len + (j * 2) + 1] = ']';
        }
        str[str_len] = '\0';
        
        return str;
      }
    }
  }
  else if (type->code == SPerl_TYPE_C_CODE_SUB) {
    
  }
}
