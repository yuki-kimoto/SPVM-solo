#include <string.h>
#include "spvm_resolved_type.h"
#include "spvm.h"
#include "spvm_allocator_parser.h"
#include "spvm_package.h"
#include "spvm_parser.h"

const char* const SPVM_RESOLVED_TYPE_C_CORE_NAMES[] = {
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double",
  "string",
  "byte[]",
  "short[]",
  "int[]",
  "long[]",
  "float[]",
  "double[]",
  "string[]",
};

int32_t SPVM_RESOLVED_TYPE_get_array_dimention(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  if (!SPVM_RESOLVED_TYPE_is_array(spvm, resolved_type)) {
    return 0;
  }
  else {
    int32_t dimention = 0;
    
    const char* resolved_type_name = resolved_type->name;
    const char* sub_end_ptr = strrchr(resolved_type_name, ')');
    
    const char* start_ptr = NULL;
    
    // Sub type
    if (sub_end_ptr == NULL) {
      start_ptr = resolved_type_name;
    }
    // Not sub type
    else {
      start_ptr = sub_end_ptr + 1;
    }
    
    while (1) {
      char* array_start_ptr = strchr(start_ptr, '[');
      if (array_start_ptr == NULL) {
        break;
      }
      else {
        dimention++;
        start_ptr = array_start_ptr + 1;
      }
    }
    
    return dimention;
  }
}

SPVM_RESOLVED_TYPE* SPVM_RESOLVED_TYPE_new(SPVM* spvm) {
  SPVM_RESOLVED_TYPE* resolved_type = SPVM_ALLOCATOR_PARSER_alloc_memory_pool(spvm, spvm->parser->allocator, sizeof(SPVM_RESOLVED_TYPE));
  
  resolved_type->part_names = SPVM_ALLOCATOR_PARSER_alloc_array(spvm, spvm->parser->allocator, 0);
  
  return resolved_type;
}

_Bool SPVM_RESOLVED_TYPE_is_array(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  int32_t length = (int32_t)strlen(resolved_type->name);
  
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

_Bool SPVM_RESOLVED_TYPE_contain_sub(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  const char* name = resolved_type->name;
  
  const char* found = strchr(name, '(');
  if (found) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_integral(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  if (resolved_type->id <= SPVM_RESOLVED_TYPE_C_ID_LONG) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_core_value_array(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  const char* name = resolved_type->name;
  
  if (strcmp(name, "char[]") == 0 || strcmp(name, "byte[]") == 0 || strcmp(name, "short[]") == 0
    || strcmp(name, "int[]") == 0 || strcmp(name, "long[]") == 0 || strcmp(name, "float[]") == 0 || strcmp(name, "double[]") == 0)
  {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_core_array(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  const char* name = resolved_type->name;
  
  if (strcmp(name, "char[]") == 0 || strcmp(name, "byte[]") == 0 || strcmp(name, "short[]") == 0
    || strcmp(name, "int[]") == 0 || strcmp(name, "long[]") == 0 || strcmp(name, "float[]") == 0 || strcmp(name, "double[]") == 0
    || strcmp(name, "string[]") == 0)
  {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_numeric(SPVM* spvm, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)spvm;
  
  if (resolved_type && resolved_type->id <= SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
    return 1;
  }
  else {
    return 0;
  }
}
