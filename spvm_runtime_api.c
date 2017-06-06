#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "spvm.h"
#include "spvm_runtime.h"
#include "spvm_runtime_api.h"
#include "spvm_runtime_allocator.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_constant_pool_package.h"
#include "spvm_constant_pool_field.h"
#include "spvm_sv.h"
#include "spvm_compat.h"
#include "spvm_value.h"
#include "spvm_data_api.h"

inline int64_t SPVM_RUNTIME_API_calcurate_data_byte_size(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data) {
  
  int64_t byte_size;
  
  // Reference is string
  if (data->type == SPVM_DATA_C_TYPE_STRING) {
    byte_size = SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + sizeof(SPVM_SV*);
  }
  // Reference is array
  else if (data->type == SPVM_DATA_C_TYPE_ARRAY) {
    SPVM_DATA_ARRAY* data_array = (SPVM_DATA_ARRAY*)data;
    byte_size = SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + data_array->length * SPVM_DATA_ARRAY_C_VALUE_SIZES[data_array->value_type];
  }
  // Reference is object
  else if (data->type == SPVM_DATA_C_TYPE_OBJECT) {
    SPVM_DATA_OBJECT* data_object = (SPVM_DATA_OBJECT*)data;
    SPVM_CONSTANT_POOL_PACKAGE constant_pool_package;
    memcpy(&constant_pool_package, &runtime->constant_pool[data_object->package_constant_pool_address], sizeof(SPVM_CONSTANT_POOL_PACKAGE));
    byte_size = SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + sizeof(SPVM_VALUE) * constant_pool_package.fields_length;
  }
  else {
    assert(0);
  }
  
  return byte_size;
}

inline void SPVM_RUNTIME_API_dec_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data) {
  (void)spvm;
  (void)runtime;
  
  if (data != NULL) {
    
    assert(data->ref_count > 0);
    
    // Decrement reference count
    data->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (data->ref_count == 0) {
      
      // Reference is string
      if (data->type == SPVM_DATA_C_TYPE_STRING) {
        SPVM_DATA_STRING* data_string = (SPVM_DATA_STRING*)data;
        
        SPVM_SV* sv = *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
        
        SPVM_SvREFCNT_dec(sv);
        SPVM_RUNTIME_ALLOCATOR_free_data(spvm, runtime->allocator, (SPVM_DATA*)data_string);
      }
      // Reference is array
      else if (data->type == SPVM_DATA_C_TYPE_ARRAY) {
        SPVM_DATA_ARRAY* data_array = (SPVM_DATA_ARRAY*)data;
        if (data_array->value_type == SPVM_DATA_ARRAY_C_VALUE_TYPE_REF) {
          
          // Array length
          int32_t length = data_array->length;
          
          for (int32_t i = 0; i < length; i++) {
            SPVM_DATA* data_element = *(SPVM_DATA**)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + sizeof(void*) * i);
            SPVM_RUNTIME_API_dec_ref_count(spvm, runtime, data_element);
          }
        }
        SPVM_RUNTIME_ALLOCATOR_free_data(spvm, runtime->allocator, data);
      }
      // Reference is object
      else if (data->type == SPVM_DATA_C_TYPE_OBJECT) {
        SPVM_DATA_OBJECT* data_object = (SPVM_DATA_OBJECT*)data;
        
        int32_t package_constant_pool_address = data_object->package_constant_pool_address;
        SPVM_CONSTANT_POOL_PACKAGE constant_pool_package;
        memcpy(&constant_pool_package, &data_object->constant_pool[package_constant_pool_address], sizeof(SPVM_CONSTANT_POOL_PACKAGE));
        
        int32_t ref_fields_length = constant_pool_package.ref_fields_length;
        
        for (int i = 0; i < ref_fields_length; i++) {
          SPVM_DATA* data_field = *(SPVM_DATA**)((intptr_t)data_object + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + sizeof(void*) * i);
          SPVM_RUNTIME_API_dec_ref_count(spvm, runtime, data_field);
        }
        
        SPVM_RUNTIME_ALLOCATOR_free_data(spvm, runtime->allocator, (SPVM_DATA*)data_object);
      }
    }
  }
}

inline void SPVM_RUNTIME_API_inc_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data) {
  (void)spvm;
  (void)runtime;
  
  if (data != NULL) {
    assert(data->ref_count >= 0);
    // Increment reference count
    data->ref_count += 1;
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

inline int32_t SPVM_RUNTIME_API_get_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data) {
  (void)spvm;
  (void)runtime;
  
  return data->ref_count;
}

inline SPVM_SV* SPVM_RUNTIME_API_get_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_STRING* data_string) {
  (void)spvm;
  (void)runtime;
  
  return *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline char* SPVM_RUNTIME_API_get_string_value(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_STRING* data_string) {
  (void)spvm;
  (void)runtime;
  
  SPVM_SV* sv = SPVM_RUNTIME_API_get_string_sv(spvm, runtime, data_string);
  assert(sv);
  char* value = SPVM_COMPAT_SVpv(sv);
  
  return value;
}

inline void* SPVM_RUNTIME_API_create_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_SV* sv) {
  (void)spvm;
  (void)runtime;
  
  SPVM_RUNTIME_ALLOCATOR* allocator = runtime->allocator;
  
  // Allocate array
  int32_t data_string_byte_size = SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE + sizeof(SPVM_SV*);
  SPVM_DATA_STRING* data_string = SPVM_RUNTIME_ALLOCATOR_malloc(spvm, allocator, data_string_byte_size);
  
  // Set constant pool
  data_string->constant_pool = runtime->constant_pool;
  
  // Set type
  data_string->type = SPVM_DATA_C_TYPE_STRING;
  
  // Set reference count
  data_string->ref_count = 0;
  
  // Set sv
  *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE) = sv;
  
  assert(data_string_byte_size == SPVM_RUNTIME_API_calcurate_data_byte_size(spvm, spvm->runtime, (SPVM_DATA*)data_string));
  
  return data_string;
}

inline SPVM_DATA_STRING* SPVM_RUNTIME_API_create_data_string_from_pv(SPVM* spvm, SPVM_RUNTIME* runtime, const char* pv) {
  (void)spvm;
  (void)runtime;
  
  // New sv
  SPVM_SV* sv = SPVM_COMPAT_newSVpvn(spvm, pv, strlen(pv));
  
  SPVM_RUNTIME_ALLOCATOR* allocator = runtime->allocator;
  
  // Allocate array
  int32_t data_string_byte_size = SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE;
  SPVM_DATA_STRING* data_string = SPVM_RUNTIME_ALLOCATOR_malloc(spvm, allocator, data_string_byte_size);
  
  // Fatal memory allocation error
  if (!data_string) {
    fprintf(stderr, "Failed to allocate memory(create_data_string_from_pv)");
    abort();
  }
  
  // Set type
  data_string->type = SPVM_DATA_C_TYPE_STRING;
  
  // Set reference count
  data_string->ref_count = 0;
  
  // Set sv
  *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE) = sv;

  assert(data_string_byte_size == SPVM_RUNTIME_API_calcurate_data_byte_size(spvm, spvm->runtime, (SPVM_DATA*)data_string));
  
  return data_string;
}
