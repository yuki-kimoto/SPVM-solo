#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "spvm_data_api.h"
#include "spvm_data.h"
#include "spvm_data_array.h"
#include "spvm_data_object.h"
#include "spvm_data_string.h"

inline SPVM_SV* SPVM_DATA_API_get_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_STRING* data_string) {
  (void)spvm;
  (void)runtime;
  
  return *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

int32_t* SPVM_DATA_API_get_constant_pool(SPVM_DATA* data) {
  
  return data->constant_pool;
}

inline int32_t SPVM_DATA_API_get_array_length(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_ARRAY* array) {
  
  return array->length;
}

inline int8_t* SPVM_DATA_API_get_array_values_byte(SPVM_DATA_ARRAY* data_array) {
  
  return (int8_t*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline int16_t* SPVM_DATA_API_get_array_values_short(SPVM_DATA_ARRAY* data_array) {
  
  return (int16_t*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline int32_t* SPVM_DATA_API_get_array_values_int(SPVM_DATA_ARRAY* data_array) {
  
  return (int32_t*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline int64_t* SPVM_DATA_API_get_array_values_long(SPVM_DATA_ARRAY* data_array) {
  
  return (int64_t*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline float* SPVM_DATA_API_get_array_values_float(SPVM_DATA_ARRAY* data_array) {
  
  return (float*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}

inline double* SPVM_DATA_API_get_array_values_double(SPVM_DATA_ARRAY* data_array) {
  
  return (double*)((intptr_t)data_array + SPVM_DATA_API_C_DATA_HEADER_BYTE_SIZE);
}
