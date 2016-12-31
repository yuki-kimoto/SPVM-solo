#include <string.h>
#include "sperl_resolved_type.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"
#include "sperl_package.h"

const char* const SPerl_RESOLVED_TYPE_C_CORE_NAMES[] = {
  "boolean",
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double"
};

const char* const SPerl_RESOLVED_TYPE_C_CORE_ARRAY_NAMES[] = {
  "boolean[]",
  "byte[]",
  "short[]",
  "int[]",
  "long[]",
  "float[]",
  "double[]"
};

int32_t const SPerl_RESOLVED_TYPE_C_CORE_SIZES[] = {
  1,
  1,
  1,
  1,
  2,
  1,
  2
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl_PARSER* parser) {
  SPerl_RESOLVED_TYPE* resolved_type = SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_RESOLVED_TYPE));
  
  resolved_type->part_names = SPerl_ALLOCATOR_new_array(parser, 0);
  
  return resolved_type;
}

_Bool SPerl_RESOLVED_TYPE_is_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  int32_t length = strlen(resolved_type->name);
  
  if (strlen(resolved_type->name) >= 2) {
    char char1 = resolved_type->name[length - 2];
    char char2 = resolved_type->name[length - 1];
    
    if (char1 == '[' && char2 == ']') {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_multi_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  int32_t length = strlen(resolved_type->name);
  
  if (strlen(resolved_type->name) >= 4) {
    char char1 = resolved_type->name[length - 4];
    char char2 = resolved_type->name[length - 3];
    char char3 = resolved_type->name[length - 2];
    char char4 = resolved_type->name[length - 1];
    
    if (char1 == '[' && char2 == ']' && char3 == '[' && char4 == ']' ) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_contain_sub(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  const char* name = resolved_type->name;
  
  const char* found = strchr(name, '(');
  if (found) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_integral(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_LONG) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_core_type_array(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  const char* name = resolved_type->name;
  
  if (strcmp(name, "boolean[]") == 0 || strcmp(name, "char[]") == 0 || strcmp(name, "byte[]") == 0 || strcmp(name, "short[]") == 0
    || strcmp(name, "int[]") == 0 || strcmp(name, "long[]") == 0 || strcmp(name, "float[]") == 0 || strcmp(name, "double[]") == 0)
  {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_core_type(SPerl_PARSER* parser, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)parser;
  
  if (resolved_type && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
    return 1;
  }
  else {
    return 0;
  }
}
