#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "spvm_sv.h"
#include "spvm_data_api.h"

const int32_t SPVM_DATA_ARRAY_C_VALUE_SIZES[] = {
  sizeof(int8_t),
  sizeof(int16_t),
  sizeof(int32_t),
  sizeof(int64_t),
  sizeof(float),
  sizeof(double),
  sizeof(void*),
};

inline SPVM_SV* SPVM_DATA_API_get_string_sv(SPVM_DATA_STRING* data_string) {
  
  return *(SPVM_SV**)((intptr_t)data_string + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline int32_t* SPVM_DATA_API_get_constant_pool(SPVM_DATA* data) {
  
  return data->constant_pool;
}

inline int32_t SPVM_DATA_API_get_ref_count(SPVM_DATA* data) {
  
  return data->ref_count;
}

inline int32_t SPVM_DATA_API_get_array_length(SPVM_DATA_ARRAY* data_array) {
  
  return data_array->length;
}

inline int8_t* SPVM_DATA_API_get_array_values_byte(SPVM_DATA_ARRAY* data_array) {
  
  return (int8_t*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline int16_t* SPVM_DATA_API_get_array_values_short(SPVM_DATA_ARRAY* data_array) {
  
  return (int16_t*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline int32_t* SPVM_DATA_API_get_array_values_int(SPVM_DATA_ARRAY* data_array) {
  
  return (int32_t*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline int64_t* SPVM_DATA_API_get_array_values_long(SPVM_DATA_ARRAY* data_array) {
  
  return (int64_t*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline float* SPVM_DATA_API_get_array_values_float(SPVM_DATA_ARRAY* data_array) {
  
  return (float*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}

inline double* SPVM_DATA_API_get_array_values_double(SPVM_DATA_ARRAY* data_array) {
  
  return (double*)((intptr_t)data_array + SPVM_DATA_C_HEADER_BYTE_SIZE);
}
