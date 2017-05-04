#ifndef SPVM_API_H
#define SPVM_API_H

enum {
  // 0
  SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET = 0,
  SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_SIZE = sizeof(int8_t),
  
  // 1
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_SIZE = sizeof(int8_t),

  // 2
  SPVM_API_C_OBJECT_HEADER_FREELIST_INDEX_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_FREELIST_INDEX_BYTE_SIZE = sizeof(int8_t),
  
  // 3
  SPVM_API_C_OBJECT_HEADER_PADDING3_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_FREELIST_INDEX_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_FREELIST_INDEX_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_PADDING3_BYTE_SIZE = sizeof(int8_t),
  
  // 4
  SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_PADDING3_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_PADDING3_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_SIZE = sizeof(int32_t),
  
  // 8
  SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_SIZE = sizeof(int64_t),
  
  // 16
  SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET = SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_SIZE,
  SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_SIZE = sizeof(intmax_t),
  
  SPVM_API_C_OBJECT_HEADER_BYTE_SIZE = SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_SIZE,
};

enum {
  SPVM_API_C_OBJECT_HEADER_TYPE_OBJECT,
  SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY,
  SPVM_API_C_OBJECT_HEADER_TYPE_STRING
};

enum {
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_SHORT,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_INT,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_LONG,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_FLOAT,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_DOUBLE,
  SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_STRING,
};

void SPVM_API_dec_ref_count(SPVM* spvm, SPVM_ENV* env, void* address);
void SPVM_API_inc_ref_count(SPVM* spvm, SPVM_ENV* env, void* address);

void SPVM_API_call_sub(SPVM* spvm, SPVM_ENV* env, const char* sub_base_name);

int64_t SPVM_API_get_array_ref_count(SPVM* spvm, SPVM_ENV* env, void* array);
int64_t SPVM_API_get_array_length(SPVM* spvm, SPVM_ENV* env, void* array);

int8_t* SPVM_API_get_array_byte_values(SPVM* spvm, SPVM_ENV* env, void* address);
int16_t* SPVM_API_get_array_short_values(SPVM* spvm, SPVM_ENV* env, void* address);
int32_t* SPVM_API_get_array_int_values(SPVM* spvm, SPVM_ENV* env, void* address);
int64_t* SPVM_API_get_array_long_values(SPVM* spvm, SPVM_ENV* env, void* address);
float* SPVM_API_get_array_float_values(SPVM* spvm, SPVM_ENV* env, void* address);
double* SPVM_API_get_array_double_values(SPVM* spvm, SPVM_ENV* env, void* address);
SPVM_SV** SPVM_API_get_array_string_values(SPVM* spvm, SPVM_ENV* env, void* address);

SPVM_SV* SPVM_API_get_string_sv(SPVM* spvm, SPVM_ENV* env, void* address);

int8_t SPVM_API_get_var_byte(SPVM* spvm, SPVM_ENV* env, int64_t index);
int16_t SPVM_API_get_var_short(SPVM* spvm, SPVM_ENV* env, int64_t index);
int32_t SPVM_API_get_var_int(SPVM* spvm, SPVM_ENV* env, int64_t index);
int64_t SPVM_API_get_var_long(SPVM* spvm, SPVM_ENV* env, int64_t index);
float SPVM_API_get_var_float(SPVM* spvm, SPVM_ENV* env, int64_t index);
double SPVM_API_get_var_double(SPVM* spvm, SPVM_ENV* env, int64_t index);
void* SPVM_API_get_var_ref(SPVM* spvm, SPVM_ENV* env, int64_t index);

void SPVM_API_push_var_byte(SPVM* spvm, SPVM_ENV* env, int8_t value);
void SPVM_API_push_var_short(SPVM* spvm, SPVM_ENV* env, int16_t value);
void SPVM_API_push_var_int(SPVM* spvm, SPVM_ENV* env, int32_t value);
void SPVM_API_push_var_long(SPVM* spvm, SPVM_ENV* env, int64_t value);
void SPVM_API_push_var_float(SPVM* spvm, SPVM_ENV* env, float value);
void SPVM_API_push_var_double(SPVM* spvm, SPVM_ENV* env, double value);
void SPVM_API_push_var_ref(SPVM* spvm, SPVM_ENV* env, void* value);

void SPVM_API_push_ret_byte(SPVM* spvm, SPVM_ENV* env, int8_t value);
void SPVM_API_push_ret_short(SPVM* spvm, SPVM_ENV* env, int16_t value);
void SPVM_API_push_ret_int(SPVM* spvm, SPVM_ENV* env, int32_t value);
void SPVM_API_push_ret_long(SPVM* spvm, SPVM_ENV* env, int64_t value);
void SPVM_API_push_ret_float(SPVM* spvm, SPVM_ENV* env, float value);
void SPVM_API_push_ret_double(SPVM* spvm, SPVM_ENV* env, double value);
void SPVM_API_push_ret_ref(SPVM* spvm, SPVM_ENV* env, void* value);

int8_t SPVM_API_pop_return_value_byte(SPVM* spvm, SPVM_ENV* env);
int16_t SPVM_API_pop_return_value_short(SPVM* spvm, SPVM_ENV* env);
int32_t SPVM_API_pop_return_value_int(SPVM* spvm, SPVM_ENV* env);
int64_t SPVM_API_pop_return_value_long(SPVM* spvm, SPVM_ENV* env);
float SPVM_API_pop_return_value_float(SPVM* spvm, SPVM_ENV* env);
double SPVM_API_pop_return_value_double(SPVM* spvm, SPVM_ENV* env);
void* SPVM_API_pop_return_value_ref(SPVM* spvm, SPVM_ENV* env);

#endif
