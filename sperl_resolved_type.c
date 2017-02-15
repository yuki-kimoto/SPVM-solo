#include <string.h>
#include "sperl_resolved_type.h"
#include "sperl.h"
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

int32_t SPerl_RESOLVED_TYPE_get_array_dimention(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
  if (!SPerl_RESOLVED_TYPE_is_array(sperl, resolved_type)) {
    return 0;
  }
  else {
    int32_t dimention = 0;
    
    const char* resolved_type_name = resolved_type->name;
    const char* sub_end_ptr = strrchr(resolved_type_name, ')');
    
    char* start_ptr = NULL;
    
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

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl* sperl) {
  SPerl_RESOLVED_TYPE* resolved_type = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_RESOLVED_TYPE));
  
  resolved_type->part_names = SPerl_ALLOCATOR_new_array(sperl, 0);
  
  return resolved_type;
}

_Bool SPerl_RESOLVED_TYPE_is_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
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

_Bool SPerl_RESOLVED_TYPE_contain_sub(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
  const char* name = resolved_type->name;
  
  const char* found = strchr(name, '(');
  if (found) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_integral(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
  if (resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_LONG) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_core_type_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
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

_Bool SPerl_RESOLVED_TYPE_is_core_type(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
  if (resolved_type && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPerl_RESOLVED_TYPE_is_calculatable_type(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type) {
  (void)sperl;
  
  if (resolved_type && resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_INT && resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
    return 1;
  }
  else {
    return 0;
  }
}