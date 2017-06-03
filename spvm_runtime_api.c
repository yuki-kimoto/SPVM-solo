#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "spvm.h"
#include "spvm_runtime.h"
#include "spvm_runtime_api.h"
#include "spvm_hash.h"
#include "spvm_bytecode_array.h"
#include "spvm_extention.h"
#include "spvm_bytecode.h"
#include "spvm_runtime_allocator.h"
#include "spvm_constant_pool.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_constant_pool_package.h"
#include "spvm_constant_pool_field.h"
#include "spvm_sv.h"
#include "spvm_compat.h"
#include "spvm_ref.h"
#include "spvm_ref_array.h"
#include "spvm_ref_object.h"
#include "spvm_ref_string.h"
#include "spvm_value.h"

#include "spvm_parser.h"

inline void SPVM_RUNTIME_API_dec_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* ref) {
  (void)spvm;
  (void)runtime;
  
  if (ref != NULL) {
    
    assert(ref->ref_count > 0);
    
    // Decrement reference count
    ref->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (ref->ref_count == 0) {
      
      // Reference is string
      if (ref->type == SPVM_REF_C_TYPE_STRING) {
        SPVM_REF_STRING* ref_string = (SPVM_REF_STRING*)ref;
        
        SPVM_SV* sv = ref_string->sv;
        SPVM_SvREFCNT_dec(sv);
        SPVM_RUNTIME_ALLOCATOR_free_ref(spvm, runtime->allocator, (SPVM_REF*)ref_string);
      }
      // Reference is array
      else if (ref->type == SPVM_REF_C_TYPE_ARRAY) {
        SPVM_REF_ARRAY* ref_array = (SPVM_REF_ARRAY*)ref;
        if (ref_array->value_type == SPVM_REF_ARRAY_C_VALUE_TYPE_REF) {
          
          // Array length
          int32_t length = ref_array->length;
          
          for (int32_t i = 0; i < length; i++) {
            SPVM_REF* ref_element = *(SPVM_REF**)((intptr_t)ref_array + sizeof(SPVM_REF_ARRAY) + sizeof(void*) * i);
            SPVM_RUNTIME_API_dec_ref_count(spvm, runtime, ref_element);
          }
        }
        SPVM_RUNTIME_ALLOCATOR_free_ref(spvm, runtime->allocator, ref);
      }
      // Reference is object
      else if (ref->type == SPVM_REF_C_TYPE_OBJECT) {
        SPVM_REF_OBJECT* ref_object = (SPVM_REF_OBJECT*)ref;
        
        int32_t package_constant_pool_address = ref_object->package_constant_pool_address;
        SPVM_CONSTANT_POOL_PACKAGE constant_pool_package;
        memcpy(&constant_pool_package, &spvm->parser->constant_pool->values[package_constant_pool_address], sizeof(SPVM_CONSTANT_POOL_PACKAGE));
        
        int32_t ref_fields_count = constant_pool_package.ref_fields_count;
        
        for (int i = 0; i < ref_fields_count; i++) {
          SPVM_REF* ref_field = *(SPVM_REF**)((intptr_t)ref_object + sizeof(SPVM_REF_OBJECT) + sizeof(void*) * i);
          SPVM_RUNTIME_API_dec_ref_count(spvm, runtime, ref_field);
        }
        
        SPVM_RUNTIME_ALLOCATOR_free_ref(spvm, runtime->allocator, (SPVM_REF*)ref_object);
      }
    }
  }
}

inline void SPVM_RUNTIME_API_inc_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* ref) {
  (void)spvm;
  (void)runtime;
  
  if (ref != NULL) {
    assert(ref->ref_count >= 0);
    // Increment reference count
    ref->ref_count += 1;
  }
}

inline void SPVM_RUNTIME_API_push_return_value_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int8_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].byte_value = value;
}

inline void SPVM_RUNTIME_API_push_return_value_short(SPVM* spvm, SPVM_RUNTIME* runtime, int16_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].short_value = value;
}

inline void SPVM_RUNTIME_API_push_return_value_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].int_value = value;
}

inline void SPVM_RUNTIME_API_push_return_value_long(SPVM* spvm, SPVM_RUNTIME* runtime, int64_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].long_value = value;
}

inline void SPVM_RUNTIME_API_push_return_value_float(SPVM* spvm, SPVM_RUNTIME* runtime, float value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].float_value = value;
}

inline void SPVM_RUNTIME_API_push_return_value_double(SPVM* spvm, SPVM_RUNTIME* runtime, double value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].double_value = value;
}

inline void SPVM_RUNTIME_API_push_ret(SPVM* spvm, SPVM_RUNTIME* runtime, void* value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].address_value = value;
}

inline int8_t SPVM_RUNTIME_API_pop_return_value_byte(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int8_t ret = runtime->call_stack[runtime->operand_stack_top].byte_value;
  runtime->operand_stack_top--;
  return ret;
}

inline int16_t SPVM_RUNTIME_API_pop_return_value_short(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int16_t ret = runtime->call_stack[runtime->operand_stack_top].short_value;
  runtime->operand_stack_top--;
  return ret;
}

inline int32_t SPVM_RUNTIME_API_pop_return_value_int(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  int32_t ret = runtime->call_stack[runtime->operand_stack_top].int_value;
  runtime->operand_stack_top--;
  return ret;
}

inline int64_t SPVM_RUNTIME_API_pop_return_value_long(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;

  int64_t ret = runtime->call_stack[runtime->operand_stack_top].long_value;
  runtime->operand_stack_top--;
  return ret;
}

inline float SPVM_RUNTIME_API_pop_return_value_float(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  float ret = runtime->call_stack[runtime->operand_stack_top].float_value;
  runtime->operand_stack_top--;
  return ret;
}

inline double SPVM_RUNTIME_API_pop_return_value_double(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  double ret = runtime->call_stack[runtime->operand_stack_top].double_value;
  runtime->operand_stack_top--;
  return ret;
}

inline void* SPVM_RUNTIME_API_pop_return_value_address(SPVM* spvm, SPVM_RUNTIME* runtime) {
  (void)spvm;
  (void)runtime;
  
  void* ret = runtime->call_stack[runtime->operand_stack_top].address_value;
  runtime->operand_stack_top--;
  return ret;
}

inline int8_t SPVM_RUNTIME_API_get_var_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].byte_value;
}

inline int16_t SPVM_RUNTIME_API_get_var_short(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].short_value;
}

inline int32_t SPVM_RUNTIME_API_get_var_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].int_value;
}

inline int64_t SPVM_RUNTIME_API_get_var_long(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].long_value;
}

inline float SPVM_RUNTIME_API_get_var_float(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].float_value;
}

inline double SPVM_RUNTIME_API_get_var_double(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].double_value;
}

inline void* SPVM_RUNTIME_API_get_var_address(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index) {
  (void)spvm;
  (void)runtime;
  
  return runtime->call_stack[runtime->call_stack_base + (size_t)index].address_value;
}

inline void SPVM_RUNTIME_API_push_var_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int8_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].byte_value = value;
}

inline void SPVM_RUNTIME_API_push_var_short(SPVM* spvm, SPVM_RUNTIME* runtime, int16_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].short_value = value;
}

inline void SPVM_RUNTIME_API_push_var_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].int_value = value;
}

inline void SPVM_RUNTIME_API_push_var_long(SPVM* spvm, SPVM_RUNTIME* runtime, int64_t value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].long_value = value;
}

inline void SPVM_RUNTIME_API_push_var_float(SPVM* spvm, SPVM_RUNTIME* runtime, float value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].float_value = value;
}

inline void SPVM_RUNTIME_API_push_var_double(SPVM* spvm, SPVM_RUNTIME* runtime, double value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].double_value = value;
}

inline void SPVM_RUNTIME_API_push_var_address(SPVM* spvm, SPVM_RUNTIME* runtime, void* value) {
  (void)spvm;
  (void)runtime;
  
  runtime->operand_stack_top++;
  runtime->call_stack[runtime->operand_stack_top].address_value = value;
}

inline int32_t SPVM_RUNTIME_API_get_array_length(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_ARRAY* array) {
  (void)spvm;
  (void)runtime;
  
  return array->length;
}

inline int32_t SPVM_RUNTIME_API_get_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* data) {
  (void)spvm;
  (void)runtime;
  
  return data->ref_count;
}

inline SPVM_SV* SPVM_RUNTIME_API_get_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_STRING* string) {
  (void)spvm;
  (void)runtime;
  
  return string->sv;
}

inline char* SPVM_RUNTIME_API_get_string_value(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_STRING* ref_string) {
  (void)spvm;
  (void)runtime;
  
  SPVM_SV* sv = SPVM_RUNTIME_API_get_string_sv(spvm, runtime, ref_string);
  assert(sv);
  char* value = SPVM_COMPAT_SVpv(sv);
  
  return value;
}

inline int8_t* SPVM_RUNTIME_API_get_array_byte_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (int8_t*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline int16_t* SPVM_RUNTIME_API_get_array_short_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (int16_t*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline int32_t* SPVM_RUNTIME_API_get_array_int_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (int32_t*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline int64_t* SPVM_RUNTIME_API_get_array_long_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (int64_t*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline float* SPVM_RUNTIME_API_get_array_float_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (float*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline double* SPVM_RUNTIME_API_get_array_double_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address) {
  (void)spvm;
  (void)runtime;
  
  return (double*)((intptr_t)address + sizeof(SPVM_REF_ARRAY));
}

inline void* SPVM_RUNTIME_API_create_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_SV* sv) {
  (void)spvm;
  (void)runtime;
  
  SPVM_RUNTIME_ALLOCATOR* allocator = runtime->allocator;
  
  // Allocate array
  int32_t ref_string_byte_size = sizeof(SPVM_REF_STRING);
  SPVM_REF_STRING* ref_string = SPVM_RUNTIME_ALLOCATOR_malloc(spvm, allocator, ref_string_byte_size);
  
  // Set type
  ref_string->type = SPVM_REF_C_TYPE_STRING;
  
  // Set reference count
  ref_string->ref_count = 0;
  
  // Set sv
  ref_string->sv = sv;

  assert(ref_string_byte_size == SPVM_REF_calcurate_byte_size(spvm, (SPVM_REF*)ref_string));
  
  return ref_string;
}

inline SPVM_REF_STRING* SPVM_RUNTIME_API_create_ref_string_from_pv(SPVM* spvm, SPVM_RUNTIME* runtime, const char* pv) {
  (void)spvm;
  (void)runtime;
  
  // New sv
  SPVM_SV* sv = SPVM_COMPAT_newSVpvn(spvm, pv, strlen(pv));
  
  SPVM_RUNTIME_ALLOCATOR* allocator = runtime->allocator;
  
  // Allocate array
  int32_t ref_string_byte_size = sizeof(SPVM_REF_STRING);
  SPVM_REF_STRING* ref_string = SPVM_RUNTIME_ALLOCATOR_malloc(spvm, allocator, ref_string_byte_size);
  
  // Fatal memory allocation error
  if (!ref_string) {
    fprintf(stderr, "Failed to allocate memory(create_ref_string_from_pv)");
    abort();
  }
  
  // Set type
  ref_string->type = SPVM_REF_C_TYPE_STRING;
  
  // Set reference count
  ref_string->ref_count = 0;
  
  // Set sv
  ref_string->sv = sv;

  assert(ref_string_byte_size == SPVM_REF_calcurate_byte_size(spvm, (SPVM_REF*)ref_string));
  
  return ref_string;
}
